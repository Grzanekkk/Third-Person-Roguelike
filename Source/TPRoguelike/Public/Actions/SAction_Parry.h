// Made by Jan Puto 2022 :D

#pragma once

#include "CoreMinimal.h"
#include "Actions/SAction.h"
#include "GameplayTagContainer.h"
#include "SAction_Parry.generated.h"

class UParticleSystemComponent;

UCLASS()
class TPROGUELIKE_API USAction_Parry : public USAction
{
	GENERATED_BODY()
	
public:
	virtual void StartAction_Implementation(AActor* Instigator) override;

	virtual void StopAction_Implementation(AActor* Instigator) override;

protected:

	UPROPERTY(EditDefaultsOnly, Category = "Rogue|Action|Parry")
	float ActionDuration = 2.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Rogue|Action|Parry|Effects")
	TObjectPtr<UParticleSystem> ParryFX;

	UPROPERTY()
	TObjectPtr<UParticleSystemComponent> ParryParticleCompnent;

	UPROPERTY(Replicated)
	FTimerHandle Parry_TimerHandle;
};
