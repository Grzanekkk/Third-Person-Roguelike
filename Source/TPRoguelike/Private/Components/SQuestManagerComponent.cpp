// Made by Jan Puto 2022 :D


#include "Components/SQuestManagerComponent.h"
#include "Objectives/SQuestDataAsset.h"
#include "Enums/SEnums_Objectives.h"
#include "Net/UnrealNetwork.h"


USQuestManagerComponent::USQuestManagerComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
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
		ActiveObjectivesState.Add(ObjectiveTag, Stat);
		
		int32 CurrentObjectiveValue = *ActiveObjectivesState.Find(ObjectiveTag);

		for (int32 i = 0; i < ServerObjectiveData.Num(); i++)
		{
			if (ServerObjectiveData[i].Tag == ObjectiveTag)
			{
				ServerObjectiveData[i].Value = CurrentObjectiveValue;
				break;
			}
		}

		OnObjectiveValueChanged.Broadcast(ObjectiveTag, CurrentObjectiveValue);
		
		if (DefalutObjectivesGoals->IsObjectiveFinished(ObjectiveTag, CurrentObjectiveValue))
		{
			OnObjectiveStateChangedEvent.Broadcast(ObjectiveTag, EObjectiveState::FINISHED);
		}
	}
}

void USQuestManagerComponent::ServerOnlyStartObjective(FGameplayTag ObjectiveTag)
{
	check(GetOwner()->HasAuthority());

	if (!ActiveObjectivesState.Contains(ObjectiveTag))
	{
		ActiveObjectivesState.Add(ObjectiveTag, 0);
		ServerObjectiveData.Add(FObjectiveReplicationData(ObjectiveTag, 0, EObjectiveState::IN_PROGRESS));

		OnObjectiveStateChangedEvent.Broadcast(ObjectiveTag, EObjectiveState::IN_PROGRESS);
	}
}

void USQuestManagerComponent::OnRep_ServerObjectiveData()
{
	for (FObjectiveReplicationData ServerObjectiveReplicationData : ServerObjectiveData)
	{
		for (FObjectiveReplicationData LocalObjectiveReplicationData : LocalObjectiveData)
		{
			if (ServerObjectiveReplicationData.Tag == LocalObjectiveReplicationData.Tag)
			{
				if (ServerObjectiveReplicationData.ObjectiveState != LocalObjectiveReplicationData.ObjectiveState)
				{
					LocalObjectiveReplicationData.ObjectiveState = ServerObjectiveReplicationData.ObjectiveState;
					OnObjectiveStateChangedEvent.Broadcast(LocalObjectiveReplicationData.Tag, LocalObjectiveReplicationData.ObjectiveState);
				}

				// We found same objectives
				if (ServerObjectiveReplicationData.Value != LocalObjectiveReplicationData.Value)
				{
					// Value changed
					int32 DeltaValue = ServerObjectiveReplicationData.Value - LocalObjectiveReplicationData.Value;

					LocalObjectiveReplicationData.Value = ServerObjectiveReplicationData.Value;
					OnObjectiveValueChanged.Broadcast(LocalObjectiveReplicationData.Tag, LocalObjectiveReplicationData.Value, DeltaValue);
				}

				break;
			}
		}
	}
}

void USQuestManagerComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(USQuestManagerComponent, ServerObjectiveData);
}