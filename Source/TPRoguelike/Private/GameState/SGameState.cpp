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