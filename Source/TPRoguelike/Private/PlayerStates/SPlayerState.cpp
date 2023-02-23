// Made by Jan Puto 2022 :D


#include "PlayerStates/SPlayerState.h"
#include "SaveSystem/SSaveGame.h"
#include "Net/UnrealNetwork.h"

bool ASPlayerState::ApplyCreditsChange(int32 CreditsDelta)
{
	if (CreditsDelta != 0)
	{
		int32 NewCredits = AvalibleCredits + CreditsDelta;

		if (NewCredits < 0)
		{
			// We dont have enaugh credits
			return false;
		}

		if (HasAuthority())
		{
			AvalibleCredits = NewCredits;

			AActor* ControlledActor = Cast<AActor>(GetPlayerController()->GetPawn());
			MulticastOnCreditsChanged(ControlledActor, AvalibleCredits, CreditsDelta);
		}
	}

	return true;
}

void ASPlayerState::MulticastOnCreditsChanged_Implementation(AActor* InstigatorActor, float NewCredits, float CreditsDelta)
{
	OnCreditsChanged.Broadcast(InstigatorActor, NewCredits, CreditsDelta);
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

void ASPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ASPlayerState, AvalibleCredits);
}