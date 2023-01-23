// Made by Jan Puto 2023 :D


#include "Quests/SQuestBase.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Components/SQuestManagerComponent.h"
#include "Enums/SEnums_Objectives.h"
#include "Net/UnrealNetwork.h"


USQuestBase::USQuestBase()
{
	QuestName = FText::FromString("Default Quest Name");

	QuestState = EQuestState::NOT_STARTED;
}

void USQuestBase::ServerOnlyStartQuest()
{
	ensure(UKismetSystemLibrary::IsServer(GetWorld()));
	
	// We only want to change this value on the server
	QuestState = EQuestState::IN_PROGRESS;

	OuterComponent->MulticastOnQuestStateChanged(GetClass(), EQuestState::IN_PROGRESS);
	
	// Some more functilonaity
}

void USQuestBase::ClientStartQuest_Implementation()
{
	// Visuals
}

void USQuestBase::ServerOnlyFinishQuest()
{
	ensure(UKismetSystemLibrary::IsServer(GetWorld()));
	// Changes state internaly in this quest class
	QuestState = EQuestState::FINISHED;

	// Notifies everyone else aboute the change
	OuterComponent->MulticastOnQuestStateChanged(GetClass(), EQuestState::FINISHED);
}

void USQuestBase::ClientFinishQuest_Implementation()
{
	// Visuals
}

bool USQuestBase::CanStartQuest()
{
	return true;
}

void USQuestBase::OnRep_QuestState()
{
	switch (QuestState)
	{
		case EQuestState::NOT_STARTED:
			break;
		case EQuestState::IN_PROGRESS:
			ClientStartQuest();
			break;
		case EQuestState::FINISHED:
			ClientFinishQuest();
			break;
		case EQuestState::FAILED:
			break;
		case EQuestState::MAX:
			break;
		default:
			break;
	}
}

void USQuestBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(USQuestBase, QuestState);
}