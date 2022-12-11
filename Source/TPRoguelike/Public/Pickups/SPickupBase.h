// Made by Jan Puto 2022 :D

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SGameplayInterface.h"
#include "SPickupBase.generated.h"

class USphereComponent;
class UParticleSystemComponent;

UCLASS(Abstract)
class TPROGUELIKE_API ASPickupBase : public AActor, public ISGameplayInterface
{
	GENERATED_BODY()
	
public:	
	ASPickupBase();

	virtual void Tick(float DeltaTime) override;

	virtual void Interact_Implementation(APawn* InstigatorPawn);

	virtual bool CanInteract_Implementation(APawn* InstigatorPawn);

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "RL|Pickup")
	int32 UseCost = 0;

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	virtual void UsePickupItem(APawn* InstigatorPawn);

	UFUNCTION()
	void AllowInteraction();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "RL|Components")
	TObjectPtr<UStaticMeshComponent> MainMeshComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "RL|Components")
	TObjectPtr<USphereComponent> CollisionSphereComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "RL|Components")
	TObjectPtr<UParticleSystemComponent > IdleParticleComponent;

	UPROPERTY(EditDefaultsOnly, Category = "RL|Effects|Particles")
	TObjectPtr<UParticleSystem> PickUpParticles;

	UPROPERTY(EditDefaultsOnly, Category = "RL|Pickup|Interaction")
	float InteractionDelay = 10.f;

	UPROPERTY()
	FTimerHandle InteractionDelay_TimerHandle;

	UPROPERTY(EditDefaultsOnly, Category = "RL|Pickup|Interaction")
	bool bCanBeInteracted = true;
};
