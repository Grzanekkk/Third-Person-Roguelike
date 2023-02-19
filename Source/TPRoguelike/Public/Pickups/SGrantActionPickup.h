// Made by Jan Puto 2022 :D

#pragma once

#include "CoreMinimal.h"
#include "Pickups/SPickupBase.h"
#include "SGrantActionPickup.generated.h"

class USAction;

UCLASS()
class TPROGUELIKE_API ASGrantActionPickup : public ASPickupBase
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Rogue|Pickup|Action")
	TSubclassOf<USAction> ActionToGrant = nullptr;

	virtual void UsePickupItem(APawn* InstigatorPawn) override;
};
