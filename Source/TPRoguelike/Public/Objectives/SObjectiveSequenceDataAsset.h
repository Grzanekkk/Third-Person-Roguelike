// Made by Jan Puto 2023 :D

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h"
#include "SObjectiveSequenceDataAsset.generated.h"

/**
 * 
 */
UCLASS()
class TPROGUELIKE_API USObjectiveSequenceDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	UFUNCTION()
	FGameplayTagContainer& GetFirstObjectives();

	UFUNCTION()
	FGameplayTagContainer& GetNextObjectives(FGameplayTag CurrentObjective);

	UFUNCTION()
	bool IsNextStepAvalible(FGameplayTag CurrentObjective);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<FGameplayTagContainer> ObjectivesOrder;
};
