// Made by Jan Puto 2022 :D

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "SPlayerController.generated.h"

class UQuestWidget;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPawnChanged, APawn*, NewPawn);


/**
 * 
 */
UCLASS()
class TPROGUELIKE_API ASPlayerController : public APlayerController
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintAssignable)
	FOnPawnChanged OnPawnChanged;

protected:
	virtual void BeginPlay() override;

	virtual void SetPawn(APawn* InPawn);

	UFUNCTION()
	void OnQuestSystemActivate();

	UPROPERTY(EditDefaultsOnly, Category = "Rogue|UI")
	TSubclassOf<UQuestWidget> QuestWidgetClass;

	UPROPERTY()
	TObjectPtr<UQuestWidget> QuestWidgetInstance;
};
