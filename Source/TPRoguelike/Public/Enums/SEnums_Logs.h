// Made by Jan Puto 2023 :D

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "SEnums_Logs.generated.h"

UENUM(BlueprintType)
enum class ERogueLogCategory : uint8
{
	LOG UMETA(DisplayName = "Log"),
	WARNING UMETA(DisplayName = "Warning"),
	SUCCESS UMETA(DisplayName = "Success"),
	ERROR UMETA(DisplayName = "Error"),
	MAX UMETA(Hidden)
};


UCLASS()
class TPROGUELIKE_API USEnums_Logs : public UObject
{
	GENERATED_BODY()
	
};
