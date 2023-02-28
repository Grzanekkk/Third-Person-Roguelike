// Made by Jan Puto 2022 :D


#include "Components/SQuestManagerComponent.h"
#include "Objectives/SQuestDataAsset.h"
#include "Enums/SEnums_Objectives.h"
#include "Objectives/SObjectiveSequenceDataAsset.h"
#include "Enums/SEnums_Logs.h"
#include "FunctionLibrary/LogsFunctionLibrary.h"
#include "Net/UnrealNetwork.h"


USQuestManagerComponent::USQuestManagerComponent()
{
	SetIsReplicatedByDefault(true);
}

void USQuestManagerComponent::ServerOnlySetObjectiveDataAndStart(USObjectiveSequenceDataAsset* StartingObjectiveSequance, USQuestDataAsset* NewObjectivesGoals)
{
	check(GetOwner()->HasAuthority());

	if (StartingObjectiveSequance && NewObjectivesGoals)
	{
		ActivateQuestSystem();

		ObjectivesGoals = NewObjectivesGoals;
		ServerOnlyStartObjectiveSequance(StartingObjectiveSequance);
	}
}

void USQuestManagerComponent::ServerOnlyAddObjectiveStat(const FGameplayTag& ObjectiveTag, int32 Stat)
{
	check(GetOwner()->HasAuthority());

	if (IsObjectiveActive(ObjectiveTag))
	{
		int32 OldObjectiveValue = GetActiveObjectiveValue(ObjectiveTag);		
		int32 NewObjectiveValue = OldObjectiveValue + Stat;

		int32 Index = 0;
		for (; Index < ServerObjectiveData.Num(); Index++)
		{
			if (ServerObjectiveData[Index].Tag == ObjectiveTag)
			{
				ChangeObjectiveValueByRef(ServerObjectiveData[Index], NewObjectiveValue);
				break;
			}
		}
		
		if (ObjectivesGoals->IsObjectiveFinished(ObjectiveTag, NewObjectiveValue))
		{
			ServerOnlyFinishObjectiveByRef(ServerObjectiveData[Index]);
		}
	}
}

void USQuestManagerComponent::ServerOnlyStartObjective(const FGameplayTag& ObjectiveTag)
{
	check(GetOwner()->HasAuthority());

	if (!ActiveObjectivesState.Contains(ObjectiveTag))
	{
		ActiveObjectivesState.Add(ObjectiveTag);
		ServerObjectiveData.Add(FObjectiveReplicationData(ObjectiveTag, 0, EObjectiveState::IN_PROGRESS));

		ChangeObjectiveStateByTag(ObjectiveTag, EObjectiveState::IN_PROGRESS);
	}
	else
	{
		FString DebugMsg = "Objective: " + ObjectiveTag.ToString() + " is already active";
		ULogsFunctionLibrary::LogOnScreen(GetWorld(), DebugMsg, ERogueLogCategory::WARNING);
	}
}

void USQuestManagerComponent::ServerOnlyFinishObjectiveByRef(FObjectiveReplicationData& FinishedObjectiveData)
{
	check(GetOwner()->HasAuthority());

	ChangeObjectiveStateByRef(FinishedObjectiveData, EObjectiveState::FINISHED);

	ServerOnlyStartNextObjectiveInSequanceIfPossible(FinishedObjectiveData.Tag);
}

void USQuestManagerComponent::ServerOnlyStartNextObjectiveInSequanceIfPossible(const FGameplayTag& FinishedObjectiveTag)
{
	check(GetOwner()->HasAuthority());

	bool bIsObjectiveInCurrentSequance = false;
	bool bIsAnyObjectiveInSequanceInProgress = false;

	// Checking if finished objective is a part of CurrentObjectiveSequanceStep
	for (FGameplayTag ObjectiveInSquanceStep : CurrentObjectiveSequanceStep)
	{
		if (ObjectiveInSquanceStep == FinishedObjectiveTag)
		{
			bIsObjectiveInCurrentSequance = true;
		}
	}

	// Checking if after finishing objective that is a part of CurrentSequance, this sequanace has any objective in progress. If not we know that we just finished this sequance.
	if (bIsObjectiveInCurrentSequance)
	{
		for (FGameplayTag ObjectiveInSquanceStep : CurrentObjectiveSequanceStep)
		{
			for (const FObjectiveReplicationData& FObjectiveReplicationData : ServerObjectiveData)
			{
				if (FObjectiveReplicationData.Tag == ObjectiveInSquanceStep)
				{
					if (FObjectiveReplicationData.ObjectiveState == EObjectiveState::IN_PROGRESS)
					{
						bIsAnyObjectiveInSequanceInProgress = true;
					}
				}
			}
		}
	}

	if (bIsObjectiveInCurrentSequance && !bIsAnyObjectiveInSequanceInProgress)
	{
		// Checking if there are any further steps in this sequance
		if (ActiveObjectiveSequance->IsNextStepAvalible(FinishedObjectiveTag))
		{
			// Now we know that we can start next step of the sequance
			CurrentObjectiveSequanceStep.Empty();

			const FGameplayTagContainer& ObjectivesInNextStep = ActiveObjectiveSequance->GetNextObjectives(FinishedObjectiveTag);
			for (int32 i = 0; i < ObjectivesInNextStep.Num(); i++)
			{
				FGameplayTag ObjectiveTag = ObjectivesInNextStep.GetByIndex(i);
				CurrentObjectiveSequanceStep.Add(ObjectiveTag);
				ServerOnlyStartObjective(ObjectiveTag);
			}
		}
		else
		{
			FString DebugMsg = "You finished all objectives in this sequance! Good job.";
			ULogsFunctionLibrary::LogOnScreen(GetWorld(), DebugMsg, ERogueLogCategory::SUCCESS);
		}
	}
}

void USQuestManagerComponent::ServerOnlyStartObjectiveSequance(TObjectPtr<USObjectiveSequenceDataAsset> ObjectiveSequance)
{
	check(GetOwner()->HasAuthority());

	CurrentObjectiveSequanceStep.Empty();
	ActiveObjectiveSequance = ObjectiveSequance;

	const FGameplayTagContainer& StartingObjectivesFromSequance = ActiveObjectiveSequance->GetFirstObjectives();
	for (int32 i = 0; i < StartingObjectivesFromSequance.Num(); i++)
	{
		FGameplayTag ObjectiveTag = StartingObjectivesFromSequance.GetByIndex(i);
		CurrentObjectiveSequanceStep.Add(ObjectiveTag);
		ServerOnlyStartObjective(ObjectiveTag);
	}
}

bool USQuestManagerComponent::IsObjectiveActive(const FGameplayTag& ObjectiveTag)
{
	return ActiveObjectivesState.Contains(ObjectiveTag);
}

bool USQuestManagerComponent::IsObjectiveFinished(const FGameplayTag& ObjectiveTag)
{
	return ObjectivesGoals->IsObjectiveFinished(ObjectiveTag, GetActiveObjectiveValue(ObjectiveTag));
}

void USQuestManagerComponent::OnRep_ServerObjectiveData()
{
	for (const FObjectiveReplicationData& ServerObjectiveReplicationData : ServerObjectiveData)
	{
		bool bIsObjectiveNewlyAdded = true;
		for (FObjectiveReplicationData& LocalObjectiveReplicationData : LocalObjectiveData)
		{
			if (ServerObjectiveReplicationData.Tag == LocalObjectiveReplicationData.Tag)
			{
				// We found same objectives
				bIsObjectiveNewlyAdded = false;

				// Checking for state change
				if (ServerObjectiveReplicationData.ObjectiveState != LocalObjectiveReplicationData.ObjectiveState)
				{
					ChangeObjectiveStateByRef(LocalObjectiveReplicationData, ServerObjectiveReplicationData.ObjectiveState);
				}

				// Checking for value change
				if (ServerObjectiveReplicationData.Value != LocalObjectiveReplicationData.Value)
				{
					// Value changed
					ChangeObjectiveValueByRef(LocalObjectiveReplicationData, ServerObjectiveReplicationData.Value);
				}

				break;
			}
		}

		if (bIsObjectiveNewlyAdded)
		{
			// if we didnt find matching Tag it means that this objective was just added
			LocalObjectiveData.Add(FObjectiveReplicationData(ServerObjectiveReplicationData.Tag, ServerObjectiveReplicationData.Value, ServerObjectiveReplicationData.ObjectiveState));
			ChangeObjectiveStateByTag(ServerObjectiveReplicationData.Tag, ServerObjectiveReplicationData.ObjectiveState);

			if (ServerObjectiveReplicationData.Value != 0)
			{
				ChangeObjectiveValueByTag(ServerObjectiveReplicationData.Tag, ServerObjectiveReplicationData.Value);
			}
		}
	}
}

void USQuestManagerComponent::OnRep_IsQuestSystemActive()
{
	if (bIsQuestSystemActive)
	{
		ActivateQuestSystem();
	}
}

void USQuestManagerComponent::ChangeObjectiveStateByRef(FObjectiveReplicationData& ObjectiveData, EObjectiveState NewState)
{
	// if we call this on the server FinishedObjectiveData should be "ServerObjectiveData", and if we call it from the clien we should pass "LocalObjectiveData"
	ObjectiveData.ObjectiveState = NewState;
	
	OnObjectiveStateChangedEvent.Broadcast(ObjectiveData.Tag, NewState);
}

void USQuestManagerComponent::ChangeObjectiveStateByTag(const FGameplayTag& ObjectiveTag, EObjectiveState NewState)
{
	if (GetOwner()->HasAuthority())
	{
		for (FObjectiveReplicationData& ServerObjectiveReplicationData : ServerObjectiveData)
		{
			if (ServerObjectiveReplicationData.Tag == ObjectiveTag)
			{
				ChangeObjectiveStateByRef(ServerObjectiveReplicationData, NewState);
				break;
			}
		}
	}
	else
	{
		for (FObjectiveReplicationData& LocalObjectiveReplicationData : LocalObjectiveData)
		{
			if (LocalObjectiveReplicationData.Tag == ObjectiveTag)
			{
				ChangeObjectiveStateByRef(LocalObjectiveReplicationData, NewState);
				break;
			}
		}
	}
}

void USQuestManagerComponent::ChangeObjectiveValueByRef(FObjectiveReplicationData& ObjectiveData, int32 NewValue)
{
	int32 OldValue = GetActiveObjectiveValue(ObjectiveData.Tag);
	int32 DeltaValue = NewValue - OldValue;
	ObjectiveData.Value = NewValue;
	OnObjectiveValueChangedEvent.Broadcast(ObjectiveData.Tag, NewValue, DeltaValue);
}

void USQuestManagerComponent::ChangeObjectiveValueByTag(const FGameplayTag& ObjectiveTag, int32 NewValue)
{
	if (GetOwner()->HasAuthority())
	{
		for (FObjectiveReplicationData& ServerObjectiveReplicationData : ServerObjectiveData)
		{
			if (ServerObjectiveReplicationData.Tag == ObjectiveTag)
			{
				ChangeObjectiveValueByRef(ServerObjectiveReplicationData, NewValue);
				break;
			}
		}
	}
	else
	{
		for (FObjectiveReplicationData& LocalObjectiveReplicationData : LocalObjectiveData)
		{
			if (LocalObjectiveReplicationData.Tag == ObjectiveTag)
			{
				ChangeObjectiveValueByRef(LocalObjectiveReplicationData, NewValue);
				break;
			}
		}
	}
}

int32 USQuestManagerComponent::GetActiveObjectiveValue(const FGameplayTag& ObjectiveTag)
{
	for (const FObjectiveReplicationData& ServerObjectiveReplicationData : ServerObjectiveData)
	{
		if (ServerObjectiveReplicationData.Tag == ObjectiveTag)
		{
			return ServerObjectiveReplicationData.Value;
			break;
		}
	}

	FString DebugMsg = "Objective: " + ObjectiveTag.ToString() + " is not active!";
	ULogsFunctionLibrary::LogOnScreen(GetWorld(), DebugMsg, ERogueLogCategory::ERROR);

	return -1;
}

EObjectiveState USQuestManagerComponent::GetActiveObjectiveState(const FGameplayTag& ObjectiveTag)
{
	for (const FObjectiveReplicationData& ServerObjectiveReplicationData : ServerObjectiveData)
	{
		if (ServerObjectiveReplicationData.Tag == ObjectiveTag)
		{
			return ServerObjectiveReplicationData.ObjectiveState;
			break;
		}
	}

	FString DebugMsg = "Objective: " + ObjectiveTag.ToString() + " is not active!";
	ULogsFunctionLibrary::LogOnScreen(GetWorld(), DebugMsg, ERogueLogCategory::ERROR);

	return EObjectiveState::NOT_STARTED;
}

void USQuestManagerComponent::ActivateQuestSystem()
{
	bIsQuestSystemActive = true;
	OnQuestSystemActivate.Broadcast();
}

void USQuestManagerComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(USQuestManagerComponent, ServerObjectiveData);
	DOREPLIFETIME(USQuestManagerComponent, ObjectivesGoals);
	DOREPLIFETIME(USQuestManagerComponent, bIsQuestSystemActive);
}