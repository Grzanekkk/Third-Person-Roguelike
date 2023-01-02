// Made by Jan Puto 2022 :D


#include "PlayerStates/SPlayerState.h"
#include "SaveSystem/SSaveGame.h"

bool ASPlayerState::AddCredits(int32 CreditsDelta)
{
	if (CreditsDelta != 0)
	{
		int32 NewCredits = AvalibleCredits + CreditsDelta;

		if (NewCredits < 0)
		{
			// We dont have enaugh credits
			return false;
		}

		AvalibleCredits = NewCredits;

		AActor* ControllecActor = Cast<AActor>(GetPlayerController()->GetPawn());
		OnCreditsChanged.Broadcast(ControllecActor, AvalibleCredits, CreditsDelta);
	}

	return true;
}

void ASPlayerState::SavePlayerState(USSaveGame* SaveObject)
{
	if (SaveObject)
	{
		SaveObject->Credits = AvalibleCredits;
	}
}

void ASPlayerState::LoadPlayerState(USSaveGame* SaveObject)
{
	if (SaveObject)
	{
		AvalibleCredits = SaveObject->Credits;
	}
}
