// Made by Jan Puto 2022 :D


#include "Components/SQuestManagerComponent.h"
#include "FunctionLibrary/AssetFunctionLibrary.h"
#include "Quests/SQuestBase.h"
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

void USQuestManagerComponent::ServerOnlyFinishQuestByClass(const TSoftClassPtr<USQuestBase>& QuestClass, bool bQuestFinishedSuccessfully)
{
	TSubclassOf<USQuestBase> QuestClassLoaded = UAssetFunctionLibrary::LoadClassSynchronousIfNeeded(QuestClass);
	if (QuestClassLoaded)
	{
		TObjectPtr<USQuestBase> CurrentActiveQuest = FindActiveQuestByClass(QuestClassLoaded);
 		if (CurrentActiveQuest)
		{
			// for now we can olny have one active quest, so we are not looking through a list of active quest
			CurrentActiveQuest->ServerOnlyFinishQuest();
			CurrentActiveQuests.Remove(CurrentActiveQuest);
			
			EQuestState FinishedQuestState = bQuestFinishedSuccessfully ? EQuestState::FINISHED : EQuestState::FAILED;

			MulticastOnQuestStateChanged(CurrentActiveQuest, FinishedQuestState);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Trying to finish quest that is not currently active"));
		}
	}
}

void USQuestManagerComponent::ServerStartObjectiveByClass_Implementation(const TSoftClassPtr<USObjectiveBase>& ObjectiveSoftClass, USQuestBase* InQuest)
{
	// KONSULTACJA Z ERYKIEM !!!
	if (InQuest->ServerOnlyStartObjectiveByClass(ObjectiveSoftClass))
	{
		TObjectPtr<USObjectiveBase> ActiveObjective = InQuest->GetActiveObjectiveByClass(ObjectiveSoftClass);
		if (ActiveObjective)
		{
			MulticastOnObjectiveStateChanged(ActiveObjective, InQuest, EObjectiveState::IN_PROGRESS);
		}
	}
	else
	{
		FString Msg = "Failed to start Objective: " + ObjectiveSoftClass.GetAssetName() + ". In Quest: " + GetNameSafe(InQuest);
		GEngine->AddOnScreenDebugMessage(-1, 0.0f, FColor::Red, Msg);
	}
}

USQuestBase* USQuestManagerComponent::FindActiveQuestByClass(const TSubclassOf<USQuestBase>& QuestClass)
{
	for (TObjectPtr<USQuestBase> ActiveQuest : CurrentActiveQuests)
	{
		if (ActiveQuest.GetClass() == QuestClass)
		{
			return ActiveQuest;
		}
	}

	return nullptr;
}

void USQuestManagerComponent::ServerStartQuestByClass_Implementation(const TSoftClassPtr<USQuestBase>& QuestClass)
{
	TSubclassOf<USQuestBase> QuestClassLoaded = UAssetFunctionLibrary::LoadClassSynchronousIfNeeded(QuestClass);
	if (QuestClassLoaded)
	{
		TObjectPtr<USQuestBase> NewQuestInstance = NewObject<USQuestBase>(this->GetOwner(), QuestClassLoaded);
		if (NewQuestInstance)
		{
			if (NewQuestInstance->CanStartQuest())
			{
				NewQuestInstance->Initialize(this);
				NewQuestInstance->ServerOnlyStartQuest();
				CurrentActiveQuests.Add(NewQuestInstance);
				MulticastOnQuestStateChanged(NewQuestInstance, EQuestState::IN_PROGRESS);
			}
		}
	}
}

void USQuestManagerComponent::MulticastOnQuestStateChanged_Implementation(USQuestBase* QuestInstance, EQuestState QuestState)
{
	OnQuestStateChanged.Broadcast(QuestInstance, QuestState);
}

void USQuestManagerComponent::MulticastOnObjectiveStateChanged_Implementation(USObjectiveBase* ObjectiveInstance, USQuestBase* QuestInstance, EObjectiveState ObjectiveState)
{
	OnObjectiveStateChanged.Broadcast(ObjectiveInstance, QuestInstance, ObjectiveState);
}

void USQuestManagerComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(USQuestManagerComponent, CurrentActiveQuests);
}