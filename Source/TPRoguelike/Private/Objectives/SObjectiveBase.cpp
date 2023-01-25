// Made by Jan Puto 2023 :D


#include "Objectives/SObjectiveBase.h"
#include "Enums/SEnums_Objectives.h"
#include "Net/UnrealNetwork.h"

USObjectiveBase::USObjectiveBase()
{
	ObjectiveState = EObjectiveState::NOT_STARTED;
	ObjectiveType = EObjectiveType::OTHER;

	ObjectiveName = FText::FromString("Default Objective Name");
}

void USObjectiveBase::ServerOnlyStartObjective()
{
	ObjectiveState = EObjectiveState::IN_PROGRESS;
}

void USObjectiveBase::ClientStartObjective_Implementation()
{
}

void USObjectiveBase::ServerOnlyFinishObjective()
{
	ObjectiveState = EObjectiveState::FINISHED;
}

void USObjectiveBase::ClientFinishObjective_Implementation()
{
}

bool USObjectiveBase::CanStartObjective()
{
	return true;
}

void USObjectiveBase::ServerOnlyOnObjectiveFinished()
{
	//OuterQuest->O
}

void USObjectiveBase::Initialize(USQuestBase* OuterQuestPtr)
{
	OuterQuest = OuterQuestPtr;
}

void USObjectiveBase::OnRep_ObjectiveState()
{
	switch (ObjectiveState)
	{
		case EObjectiveState::NOT_STARTED:
			break;
		case EObjectiveState::IN_PROGRESS:
			ClientStartObjective();
			break;
		case EObjectiveState::FINISHED:
			ClientFinishObjective();
			break;
		case EObjectiveState::FAILED:
			break;
		case EObjectiveState::MAX:
			break;
		default:
			break;
	}
}

void USObjectiveBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(USObjectiveBase, ObjectiveState);
}