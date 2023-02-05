// Made by Jan Puto 2023 :D

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h"
#include "SQuestDataAsset.generated.h"

USTRUCT(Blueprintable)
struct FObjectiveDefaults
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FGameplayTag ObjectiveTag;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 GoalValue;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
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
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<FObjectiveDefaults> ObjectivesGoal;

	UFUNCTION()
	int32 GetGoalValueOfObjective(FGameplayTag ObjectiveTag);

	UFUNCTION()
	bool IsObjectiveFinished(FGameplayTag ObjectiveTag, int32 CurrentValue);
};
