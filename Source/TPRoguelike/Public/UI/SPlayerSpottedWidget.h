// Made by Jan Puto 2022 :D

#pragma once

#include "CoreMinimal.h"
#include "UI/SWorldUserWidget.h"
#include "SPlayerSpottedWidget.generated.h"

/**
 * 
 */
UCLASS()
class TPROGUELIKE_API USPlayerSpottedWidget : public USWorldUserWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "RL|UI")
	void PlayIconShakeAnimation();
};
