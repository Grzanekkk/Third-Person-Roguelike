// Made by Jan Puto 2023 :D

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameplayTagContainer.h"
#include "ObjectiveWidget.generated.h"

class UVertivalBox;

/**
 * 
 */
UCLASS()
class TPROGUELIKE_API UObjectiveWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UObjectiveWidget();

	void Initialize(FGameplayTag ObjectiveAttached);

protected:
	UPROPERTY()
	bool bFoundMatchingObjectiveGoals = false;
};
