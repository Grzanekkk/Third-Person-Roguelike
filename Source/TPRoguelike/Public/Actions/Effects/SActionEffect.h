// Made by Jan Puto 2022 :D

#pragma once

#include "CoreMinimal.h"
#include "Actions/SAction.h"
#include "SActionEffect.generated.h"

/**
 * 
 */
UCLASS()
class TPROGUELIKE_API USActionEffect : public USAction
{
	GENERATED_BODY()

public:

	USActionEffect();

	void StartAction_Implementation(AActor* Instigator) override;

	void StopAction_Implementation(AActor* Instigator) override;
	
protected:

	UFUNCTION(BlueprintNativeEvent, Category = "Action|Effect")
	void ExecutePeriodicEffect(AActor* Instigator);

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Action|Effect")
	float Duration = 0.0f;

	// Time between ticks to apply effect
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Action|Effect")
	float Period = 0.0f;

	FTimerHandle DurationTimerHandle;
	FTimerHandle PeriodTimerHandle;

};
