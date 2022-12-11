// Made by Jan Puto 2022 :D

#pragma once

#include "CoreMinimal.h"
#include "Pickups/SPickupBase.h"
#include "SCoinPickup.generated.h"

/**
 * 
 */
UCLASS()
class TPROGUELIKE_API ASCoinPickup : public ASPickupBase
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "RL|Pickup|Coins")
	int32 CreditsGrantedOnInteraction = 5;

	virtual void UsePickupItem(APawn* InstigatorPawn) override;
};
