// Made by Jan Puto 2023 :D

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h"
#include "SQuestDataAsset.generated.h"

USTRUCT()
struct FQuestObjective
{
	GENERATED_BODY()

public:
	UPROPERTY()
	FGameplayTag ObjectiveTag;

	UPROPERTY()
	int32 GoalValue;

	UPROPERTY()
	FText DisplayName;

};

/**
 * 
 */
UCLASS(Blueprintable)
class TPROGUELIKE_API USQuestDataAsset : public UDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere)
	TMap<FGameplayTag, int32> ObjectiveState;
};
