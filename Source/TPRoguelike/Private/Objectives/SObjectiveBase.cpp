// Made by Jan Puto 2023 :D


#include "Objectives/SObjectiveBase.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Quests/SQuestBase.h"
#include "Enums/SEnums_Objectives.h"
#include "Components/SQuestManagerComponent.h"
#include "Net/UnrealNetwork.h"

//USObjectiveBase::USObjectiveBase()
//{
//	ObjectiveState = EObjectiveState::NOT_STARTED;
//	ObjectiveType = EObjectiveType::OTHER;
//
//	ObjectiveName = FText::FromString("Default Objective Name");
//}
//
//void USObjectiveBase::ServerOnlyStartObjective()
//{
//	ensure(UKismetSystemLibrary::IsServer(GetWorld()));
//
//	ObjectiveState = EObjectiveState::IN_PROGRESS;
//	OnRep_ObjectiveState();
//}
//
//void USObjectiveBase::ClientStartObjective_Implementation()
//{
//}
//
//void USObjectiveBase::ServerOnlyFinishObjective()
//{
//	ensure(UKismetSystemLibrary::IsServer(GetWorld()));
//
//	ObjectiveState = EObjectiveState::FINISHED;
//	OnRep_ObjectiveState();
//}
//
//void USObjectiveBase::ClientFinishObjective_Implementation()
//{
//}
//
//bool USObjectiveBase::CanStartObjective()
//{
//	return true;
//}
//
//void USObjectiveBase::ServerOnlyOnObjectiveFinished()
//{
//	ensure(UKismetSystemLibrary::IsServer(GetWorld()));
//
//	OuterQuest->ServerOnlyOnObjectiveFinished(this, true);
//}
//
//void USObjectiveBase::Initialize(USQuestBase* OuterQuestPtr)
//{
//	OuterQuest = OuterQuestPtr;
//}
//
//void USObjectiveBase::OnRep_ObjectiveState()
//{
//	OuterQuest->GetOuterComponent()->OnObjectiveStateChanged(this, OuterQuest, ObjectiveState);
//
//	switch (ObjectiveState)
//	{
//		case EObjectiveState::NOT_STARTED:
//			break;
//		case EObjectiveState::IN_PROGRESS:
//			ClientStartObjective();
//			break;
//		case EObjectiveState::FINISHED:
//			ClientFinishObjective();
//			break;
//		case EObjectiveState::FAILED:
//			break;
//		case EObjectiveState::MAX:
//			break;
//		default:
//			break;
//	}
//}
//
//void USObjectiveBase::OnRep_OuterQuest()
//{
//	ObjectiveName = FText::FromString("BITCH WORK");
//}
//
//bool USObjectiveBase::IsSupportedForNetworking() const
//{
//	return true;
//}
//
//void USObjectiveBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
//{
//	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
//
//	DOREPLIFETIME(USObjectiveBase, OuterQuest);
//	DOREPLIFETIME(USObjectiveBase, ObjectiveState);
//}