// Made by Jan Puto 2023 :D

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameplayTagContainer.h"
#include "ObjectiveWidget.generated.h"

class UTextBlock;
class UHorizontalBox;
class UCheckBox;

/**
 * 
 */
UCLASS()
class TPROGUELIKE_API UObjectiveWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void Initialize(const FGameplayTag& ObjectiveAttached);

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UHorizontalBox> TriggerObjectiveBox;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UHorizontalBox> StatObjectiveBox;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UCheckBox> ObjectiveFinishedCheckBox;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UTextBlock> StatObjectiveTEXT;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UTextBlock> ObjectiveNameTEXT;

protected:
	UFUNCTION()
	void OnObjectiveStateChanged(const FGameplayTag& ObjectiveTag, EObjectiveState ObjectiveState);

	UFUNCTION()
	void OnObjectiveValueChanged(const FGameplayTag& ObjectiveTag, int32 CurrentValue, int32 DeltaValue);

	UPROPERTY(EditDefaultsOnly, Category = "Rogue|Settings|Visuals")
	FSlateColor CompletedObjectiveColor;

	UPROPERTY()
	FGameplayTag ObjectiveAttached;

	UPROPERTY()
	bool bFoundMatchingObjectiveGoals = false;

	UPROPERTY()
	bool bIsStatObjective = false;

	UPROPERTY()
	int32 StatObjectiveGoalValue = 0;
};
