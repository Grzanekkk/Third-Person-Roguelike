// Made by Jan Puto 2023 :D


#include "Quests/SQuest_CaptureZone.h"
#include "Kismet/GameplayStatics.h"
#include "GameState/SGameState.h"
#include "ObjectiveArea/CaptureZone.h"


USQuest_CaptureZone::USQuest_CaptureZone()
{
	QuestName = FText::FromString("Capture Zone!");
}


void USQuest_CaptureZone::ServerOnlyStartQuest()
{
	Super::ServerOnlyStartQuest();

	TObjectPtr<ASGameState> GameState = Cast<ASGameState>(UGameplayStatics::GetGameState(GetWorld()));
	if (GameState)
	{
		TObjectPtr<ACaptureZone> CaptureZone = GameState->GetCaptureZoneForQuest();
		if (CaptureZone)
		{
			CaptureZone->ServerOnlyInitializeForQuest();
			CaptureZone->OnZoneCaptured.AddDynamic(this, &USQuest_CaptureZone::OnZoneCaptured);
		}
	}
}

void USQuest_CaptureZone::ServerOnlyFinishQuest()
{
	Super::ServerOnlyFinishQuest();
}

bool USQuest_CaptureZone::CanStartQuest()
{
	Super::CanStartQuest();
}

void USQuest_CaptureZone::OnZoneCaptured(ACaptureZone* CapturedZone, TArray<ASCharacter*> PlayersResponsibleForCapture)
{
	// Binded only on server so this will run only on the server
	CapturedZone->ServerOnlyAssginedQuestFinished();

	TObjectPtr<ASGameState> GameState = Cast<ASGameState>(UGameplayStatics::GetGameState(GetWorld()));
	if (GameState)
	{
		GameState->ServerOnlyRemoveCaptureZoneFromActiveList(CapturedZone);
	}

	ServerOnlyFinishQuest();
}