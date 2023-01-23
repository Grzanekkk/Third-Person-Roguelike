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
				NewQuestInstance->OuterComponent = this;
				NewQuestInstance->ServerOnlyStartQuest();
				CurrentActiveQuest = NewQuestInstance;
				MulticastOnQuestStateChanged(QuestClassLoaded, EQuestState::IN_PROGRESS);
			}
		}
	}
}


void USQuestManagerComponent::ServerOnQuestStateChanged_Implementation(TSubclassOf<USQuestBase> QuestClass, EQuestState QuestState)
{
	// Useless function ???
	MulticastOnQuestStateChanged(QuestClass, QuestState);
}


void USQuestManagerComponent::MulticastOnQuestStateChanged_Implementation(TSubclassOf<USQuestBase> QuestClass, EQuestState QuestState)
{
	OnQuestStateChanged.Broadcast(QuestClass, QuestState);
}

void USQuestManagerComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(USQuestManagerComponent, CurrentActiveQuest);
}