// Made by Jan Puto 2022 :D


#include "Components/SQuestManagerComponent.h"
#include "Objectives/SQuestDataAsset.h"
#include "Enums/SEnums_Objectives.h"
#include "Net/UnrealNetwork.h"


USQuestManagerComponent::USQuestManagerComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	SetIsReplicatedByDefault(true);
}

void USQuestManagerComponent::BeginPlay()
{
	Super::BeginPlay();
}

void USQuestManagerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void USQuestManagerComponent::ServerOnlyAddObjectiveStat(FGameplayTag ObjectiveTag, int32 Stat)
{
	check(GetOwner()->HasAuthority());

	// We check if this objective is active
	if (ActiveObjectivesState.Contains(ObjectiveTag))
	{
		int32 OldObjectiveValue = GetValueOfActiveObjective(ObjectiveTag);		
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
		
		if (DefalutObjectivesGoals->IsObjectiveFinished(ObjectiveTag, NewObjectiveValue))
		{
			ChangeObjectiveStateByRef(ServerObjectiveData[Index], EObjectiveState::FINISHED);
		}
	}
}

void USQuestManagerComponent::ServerOnlyStartObjective(FGameplayTag ObjectiveTag)
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
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, DebugMsg);
	}
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

void USQuestManagerComponent::ChangeObjectiveStateByRef(FObjectiveReplicationData& ObjectiveData, EObjectiveState NewState)
{
	// if we call this on the server ObjectiveData should be "ServerObjectiveData", and if we call it from the clien we should pass "LocalObjectiveData"
	ObjectiveData.ObjectiveState = NewState;
	
	OnObjectiveStateChangedEvent.Broadcast(ObjectiveData.Tag, NewState);
}

void USQuestManagerComponent::ChangeObjectiveStateByTag(FGameplayTag ObjectiveTag, EObjectiveState NewState)
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
	int32 OldValue = GetValueOfActiveObjective(ObjectiveData.Tag);
	int32 DeltaValue = NewValue - OldValue;
	ObjectiveData.Value = NewValue;
	OnObjectiveValueChanged.Broadcast(ObjectiveData.Tag, NewValue, DeltaValue);
}

void USQuestManagerComponent::ChangeObjectiveValueByTag(FGameplayTag ObjectiveTag, int32 NewValue)
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

int32 USQuestManagerComponent::GetValueOfActiveObjective(FGameplayTag ObjectiveTag)
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
	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, DebugMsg);

	return -1;
}

void USQuestManagerComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(USQuestManagerComponent, ServerObjectiveData);
}