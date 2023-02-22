// Made by Jan Puto 2023 :D

#pragma once

#include "CoreMinimal.h"
#include "Zones/SZone_Base.h"
#include "SZone_Box.generated.h"

class UBoxComponent;

/**
 * 
 */
UCLASS()
class TPROGUELIKE_API ASZone_Box : public ASZone_Base
{
	GENERATED_BODY()

public:
	ASZone_Box();
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Rogue|Components")
	TObjectPtr<UBoxComponent> TriggerBox = nullptr;
};
