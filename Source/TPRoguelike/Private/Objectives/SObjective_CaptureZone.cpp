// Made by Jan Puto 2023 :D


#include "Objectives/SObjective_CaptureZone.h"
#include "Kismet/GameplayStatics.h"
#include "GameState/SGameState.h"
#include "ObjectiveArea/CaptureZone.h"

USObjective_CaptureZone::USObjective_CaptureZone()
{
	ObjectiveName = FText::FromString("Capture Zone!");
}

void USObjective_CaptureZone::ServerOnlyStartObjective()
{
	Super::ServerOnlyStartObjective();

	TObjectPtr<ASGameState> GameState = Cast<ASGameState>(UGameplayStatics::GetGameState(GetWorld()));
	if (GameState)
	{
		TObjectPtr<ACaptureZone> CaptureZone = GameState->GetCaptureZoneForQuest();
		if (CaptureZone)
		{
			CaptureZone->ServerOnlyInitializeForQuest();
			CaptureZone->OnZoneCaptured.AddDynamic(this, &USObjective_CaptureZone::OnZoneCaptured);

			GameState->ServerOnlyRemoveCaptureZoneFromActiveList(CaptureZone);
		}
	}
}

void USObjective_CaptureZone::ServerOnlyFinishObjective()
{
	Super::ServerOnlyFinishObjective();
}

bool USObjective_CaptureZone::CanStartObjective()
{
	if (Super::CanStartObjective())
	{
		TObjectPtr<ASGameState> GameState = Cast<ASGameState>(UGameplayStatics::GetGameState(GetWorld()));
		if (GameState && GameState->IsAnyCaptureZoneAvalibleForQuest())
		{
			return true;
		}
	}

	return false;
}

void USObjective_CaptureZone::OnZoneCaptured(ACaptureZone* CapturedZone, TArray<ASCharacter*> PlayersResponsibleForCapture)
{
	// Binded only on server so this will run only on the server
	CapturedZone->ServerOnlyAssginedQuestFinished();

	TObjectPtr<ASGameState> GameState = Cast<ASGameState>(UGameplayStatics::GetGameState(GetWorld()));
	if (GameState)
	{
		GameState->ServerOnlyRemoveCaptureZoneFromActiveList(CapturedZone);
	}

	//ServerOnlyFinishQuest();
	ServerOnlyOnObjectiveFinished();
}