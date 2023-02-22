// Made by Jan Puto 2023 :D

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "SEnums_Zones.generated.h"

UENUM(BlueprintType)
enum class EZoneShape : uint8
{
	BOX UMETA(DisplayName = "Box"),
	CAPSULE UMETA(DisplayName = "Capsule"),
	MAX UMETA(Hidden)
};


UCLASS()
class TPROGUELIKE_API USEnums_Zones: public UObject
{
	GENERATED_BODY()
	
};
