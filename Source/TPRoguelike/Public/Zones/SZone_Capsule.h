// Made by Jan Puto 2023 :D

#pragma once

#include "CoreMinimal.h"
#include "Zones/SZone_Base.h"
#include "SZone_Capsule.generated.h"

class UCapsuleComponent;

/**
 * 
 */
UCLASS()
class TPROGUELIKE_API ASZone_Capsule : public ASZone_Base
{
	GENERATED_BODY()
	
public:
	ASZone_Capsule();

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Rogue|Components")
	TObjectPtr<UCapsuleComponent> TriggerCapsule = nullptr;
};
