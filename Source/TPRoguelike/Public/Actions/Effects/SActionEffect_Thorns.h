// Made by Jan Puto 2022 :D

#pragma once

#include "CoreMinimal.h"
#include "Actions/Effects/SActionEffect.h"
#include "SActionEffect_Thorns.generated.h"


UCLASS()
class TPROGUELIKE_API USActionEffect_Thorns : public USActionEffect
{
	GENERATED_BODY()
	
public:
	void StartAction_Implementation(AActor* Instigator) override;

	void StopAction_Implementation(AActor* Instigator) override;

protected:
	UFUNCTION()
	void OnOwnerHealthChanged(AActor* InstigatorActor, USAttributeComponent* OwningComp, float NewHealth, float DeltaHealth);

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Action|Effect|Thorns")
	float ThornsMultiplier = 0.15f;
};
