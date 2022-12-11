// Made by Jan Puto 2022 :D

#pragma once

#include "CoreMinimal.h"
#include "Actions/SAction_ProjectileAttack.h"
#include "SAction_Blackhole_Attack.generated.h"

/**
 * 
 */
UCLASS()
class TPROGUELIKE_API USAction_Blackhole_Attack : public USAction_ProjectileAttack
{
	GENERATED_BODY()
	
public:
	bool CanStart_Implementation(AActor* Instigator) override;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Action|Cost")
	float RageCost = 20.f;
};
