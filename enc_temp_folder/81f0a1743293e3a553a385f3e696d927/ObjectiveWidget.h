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

	UPROPERTY()
	FGameplayTag ObjectiveAttached;

	UPROPERTY()
	bool bFoundMatchingObjectiveGoals = false;
};
