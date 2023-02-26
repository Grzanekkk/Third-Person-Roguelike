// Made by Jan Puto 2023 :D


#include "Zones/SZone_AllPlayersInside.h"
#include "Kismet/GameplayStatics.h"
#include "GameState/SGameState.h"
#include "FunctionLibrary/GameplayFunctionLibrary.h"

void ASZone_AllPlayersInside::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	OnNumberOfPlayersInsideZoneChanged.AddDynamic(this, &ASZone_AllPlayersInside::CheckIfAllPlayersAreInside);
}

void ASZone_AllPlayersInside::CheckIfAllPlayersAreInside(int32 PlayersInside)
{
	TObjectPtr<ASGameState> SGameState = UGameplayFunctionLibrary::GetSGameState(GetWorld());
	if (SGameState)
	{
		if (PlayersInside >= SGameState->GetNumberOfConnectedPlayers())
		{
			if (UKismetSystemLibrary::IsServer(GetWorld()))
			{
				UGameplayFunctionLibrary::AddObjectiveStat(GetWorld(), ObjectiveTag, 1);

				UGameplayFunctionLibrary::IntaractIfPossible(GetWorld(), ActorToInteractOnCapture, nullptr);
			}

			// we might do some visuals here
		}
	}
}