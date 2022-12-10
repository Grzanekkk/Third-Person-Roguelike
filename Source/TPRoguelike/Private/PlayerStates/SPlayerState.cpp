// Made by Jan Puto 2022 :D


#include "PlayerStates/SPlayerState.h"

bool ASPlayerState::AddCredits(int32 CreditsDelta)
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
	return true;
}
