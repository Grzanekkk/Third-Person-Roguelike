// Made by Jan Puto 2023 :D


#include "Quests/SQuestBase.h"
#include "Objectives/SObjectiveBase.h"
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
	
	// Starting default objectives
	for (TSoftClassPtr<USObjectiveBase> ObjectiveSoftClass : StartingObjectives)
	{
		// @FIXME
		// I know we are calling RPC from the server
		OuterComponent->ServerStartObjectiveByClass(ObjectiveSoftClass, this);
	}
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
}

void USQuestBase::ClientFinishQuest_Implementation()
{
	// Visuals
}

void USQuestBase::ServerOnlyOnAllObjectivesFinished()
{
	ensure(UKismetSystemLibrary::IsServer(GetWorld()));
	// For now we are setting true by defalut
	OuterComponent->ServerOnlyFinishQuestByClass(GetClass(), true);
}

void USQuestBase::ServerOnlyOnObjectiveFinished(USObjectiveBase* ObjectiveInstance, bool bObjectiveFinishedSuccessfully)
{
	ensure(UKismetSystemLibrary::IsServer(GetWorld()));

	OuterComponent->ServerOnlyFinishObjective(ObjectiveInstance, this, bObjectiveFinishedSuccessfully);

	// Start next objective if exist
	if (false)
	{
		// We have another objective in this quest and we are goint to start it now
	}
	else
	{
		ServerOnlyOnAllObjectivesFinished();
	}
}

void USQuestBase::ClientFinishObjective_Implementation()
{

}

void USQuestBase::Initialize(USQuestManagerComponent* InOuterComponent)
{
	OuterComponent = InOuterComponent;
}

USObjectiveBase* USQuestBase::GetActiveObjectiveByClass(const TSoftClassPtr<USObjectiveBase>& ObjectiveSoftClass)
{
	for (TObjectPtr<USObjectiveBase> ActiveObjective : ActiveObjectives)
	{
		if (ActiveObjective.GetClass() == ObjectiveSoftClass)
		{
			return ActiveObjective;
		}
	}

	return nullptr;
}

bool USQuestBase::CanStartQuest()
{
	return true;
}

bool USQuestBase::ServerOnlyStartObjectiveByClass(const TSoftClassPtr<USObjectiveBase>& ObjectiveSoftClass)
{
	TSubclassOf<USObjectiveBase> ObjectiveClass = ObjectiveSoftClass.LoadSynchronous();
	TObjectPtr<USObjectiveBase> NewObjectiveInstance = NewObject<USObjectiveBase>(OuterComponent->GetOwner(), ObjectiveClass);
	if (NewObjectiveInstance && NewObjectiveInstance->CanStartObjective())
	{
		ActiveObjectives.Add(NewObjectiveInstance);

		NewObjectiveInstance->Initialize(this);
		NewObjectiveInstance->ServerOnlyStartObjective();

		return true;
	}

	return false;
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

bool USQuestBase::IsSupportedForNetworking() const
{
	return true;
}

void USQuestBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(USQuestBase, QuestState);
	DOREPLIFETIME(USQuestBase, ActiveObjectives);
}