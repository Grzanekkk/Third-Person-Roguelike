// Made by Jan Puto 2022 :D


#include "Pickups/SCoinPickup.h"
#include "Characters/SCharacter.h"
#include "PlayerStates/SPlayerState.h"

void ASCoinPickup::UsePickupItem(APawn* InstigatorPawn)
{
	Super::UsePickupItem(InstigatorPawn);

	if (InstigatorPawn)
	{
		TObjectPtr<ASCharacter>	InstigatorCharacter = Cast<ASCharacter>(InstigatorPawn);
		if (InstigatorCharacter)
		{
			InstigatorCharacter->GetPlayerState<ASPlayerState>()->AddCredits(CreditsGrantedOnInteraction);
		}
	}
}
