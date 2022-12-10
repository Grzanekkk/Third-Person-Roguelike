// Made by Jan Puto 2022 :D

#pragma once

#include "CoreMinimal.h"
#include "SPickupBase.h"
#include "SHealingPotion.generated.h"

UCLASS()
class TPROGUELIKE_API ASHealingPotion : public ASPickupBase
{
	GENERATED_BODY()
	
public:
	ASHealingPotion();

	virtual void Interact_Implementation(APawn* InstigatorPawn) override;

	virtual bool CanInteract_Implementation(APawn* InstigatorPawn) override;

	UPROPERTY(EditDefaultsOnly, Category = "Pickup|Healing")
	float HealingAmount = 50;

protected:
	virtual void BeginPlay() override;

	virtual void UsePickupItem(APawn* InstigatorPawn) override;
};
