// Made by Jan Puto 2023 :D

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameplayTagContainer.h"
#include "QuestWidget.generated.h"

class UVerticalBox;
class UObjectiveWidget;
enum class EObjectiveState : uint8;

/**
 * 
 */
UCLASS()
class TPROGUELIKE_API UQuestWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UVerticalBox> ObjectiveBox;

	UFUNCTION()
	void OnQuestSystemActivate();

protected:
	UFUNCTION()
	void OnObjectiveStateChanged(const FGameplayTag& ObjectiveTag, EObjectiveState ObjectiveState);

	UPROPERTY(EditDefaultsOnly, Category = "Rogue|UI")
	TSubclassOf<UObjectiveWidget> ObjectiveWidgetClass;

	UPROPERTY()
	TArray<TObjectPtr<UObjectiveWidget>> ObjectiveWidgets;
};
