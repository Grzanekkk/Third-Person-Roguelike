// Made by Jan Puto 2022 :D

#pragma once

#include "CoreMinimal.h"
#include "Actions/Effects/SActionEffect.h"
#include "SActionEffect_Healing.generated.h"

/**
 * 
 */
UCLASS()
class TPROGUELIKE_API USActionEffect_Healing : public USActionEffect
{
	GENERATED_BODY()
	
protected:
	void ExecutePeriodicEffect_Implementation(AActor* Instigator) override;
};
