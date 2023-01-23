// Made by Jan Puto 2023 :D

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "SEnums_Objectives.generated.h"

UENUM(BlueprintType)
enum class EQuestState : uint8
{
	NOT_STARTED UMETA(DisplayName = "Not Started"),
	IN_PROGRESS UMETA(DisplayName = "In Progress"),
	FINISHED UMETA(DisplayName = "Finished"),
	FAILED UMETA(DisplayName = "Failed"),
	MAX UMETA(Hidden)
};

UCLASS()
class TPROGUELIKE_API USEnums_Quests : public UObject
{
	GENERATED_BODY()
	
};
