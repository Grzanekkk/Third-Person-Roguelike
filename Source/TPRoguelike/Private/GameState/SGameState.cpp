// Made by Jan Puto 2022 :D


#include "GameState/SGameState.h"
#include "Components/SQuestManagerComponent.h"
#include "ObjectiveArea/CaptureZone.h"
#include "Net/UnrealNetwork.h"

ASGameState::ASGameState()
{
	QuestManagerComponent = CreateDefaultSubobject<USQuestManagerComponent>(TEXT("Objective Manager Component"));
}

int32 ASGameState::GetNumberOfConnectedPlayers() const
{
	return PlayerArray.Num();
}

//TObjectPtr<ACaptureZone> ASGameState::GetCaptureZoneForQuest()
//{
//	bool bZoneFound = false;
//	TObjectPtr<ACaptureZone> QuestCaptureZone;
//
//	if (IsAnyCaptureZoneAvalibleForQuest())
//	{
//		int32 Rand = FMath::RandRange(0, CaptureZonesAvalibeForQuests.Num() - 1);
//		QuestCaptureZone = CaptureZonesAvalibeForQuests[Rand];
//	}
//
//	return QuestCaptureZone;
//}
//
//void ASGameState::ServerOnlyAddCaptureZoneToActiveList(ACaptureZone* CaptureZone)
//{
//	//AUTHORITY_ONLY_FUNCTION(ServerOnlyAddCaptureZoneToActiveList)
//
//	CaptureZonesAvalibeForQuests.Add(CaptureZone);
//}
//
//void ASGameState::ServerOnlyRemoveCaptureZoneFromActiveList(ACaptureZone* CaptureZone)
//{
//	CaptureZonesAvalibeForQuests.Remove(CaptureZone);
//}
//
//bool ASGameState::IsAnyCaptureZoneAvalibleForQuest()
//{
//	return CaptureZonesAvalibeForQuests.Num() > 0;
//}
//
//void ASGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
//{
//	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
//
//	DOREPLIFETIME(ASGameState, CaptureZonesAvalibeForQuests);
//}