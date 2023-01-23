// Made by Jan Puto 2022 :D


#include "GameState/SGameState.h"
#include "Components/SQuestManagerComponent.h"
#include "ObjectiveArea/CaptureZone.h"
#include "Net/UnrealNetwork.h"

ASGameState::ASGameState()
{
	QuestManagerComponent = CreateDefaultSubobject<USQuestManagerComponent>(TEXT("Objective Manager Component"));
}

TObjectPtr<ACaptureZone> ASGameState::GetCaptureZoneForQuest()
{
	bool bZoneFound = false;
	FQuestCaptureZone QuestCaptureZone;

	while (!bZoneFound)
	{
		int32 Rand = FMath::RandRange(0, ActiveCaptureZonesOnTheMap.Num() - 1);
		QuestCaptureZone = ActiveCaptureZonesOnTheMap[Rand];
		if (!QuestCaptureZone.bHasQuestAssigned)
		{
			bZoneFound = true;
		}
	}

	return QuestCaptureZone.CaptureZone;
}

void ASGameState::ServerOnlyAddCaptureZoneToActiveList(ACaptureZone* CaptureZone)
{
	//AUTHORITY_ONLY_FUNCTION(ServerOnlyAddCaptureZoneToActiveList)

	FQuestCaptureZone QuestCaptureZone;
	QuestCaptureZone.CaptureZone = CaptureZone;
	QuestCaptureZone.bHasQuestAssigned = false;
	ActiveCaptureZonesOnTheMap.Add(QuestCaptureZone);
}

void ASGameState::ServerOnlyRemoveCaptureZoneFromActiveList(ACaptureZone* CaptureZone)
{
}

void ASGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ASGameState, ActiveCaptureZonesOnTheMap);
}