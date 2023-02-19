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

	virtual void Interact_Implementation(APawn* InstigatorPawn);

	virtual bool CanInteract_Implementation(APawn* InstigatorPawn);

	virtual bool IsEnabled_Implementation();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Rogue|Pickup")
	int32 UseCost = 0;

protected:

	UFUNCTION()
	virtual void UsePickupItem(APawn* InstigatorPawn);

	UFUNCTION(NetMulticast, Reliable)
	virtual void MulticastUsePickupItem(APawn* InstigatorPawn);

	UFUNCTION()
	void AllowInteraction();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Rogue|Components")
	TObjectPtr<UStaticMeshComponent> MainMeshComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Rogue|Components")
	TObjectPtr<USphereComponent> CollisionSphereComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Rogue|Components")
	TObjectPtr<USphereComponent> RootSphereComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Rogue|Components")
	TObjectPtr<UParticleSystemComponent > IdleParticleComponent;

	UPROPERTY(EditDefaultsOnly, Category = "Rogue|Effects|Particles")
	TObjectPtr<UParticleSystem> PickUpParticles;

	UPROPERTY(EditDefaultsOnly, Category = "Rogue|Pickup|Interaction")
	float InteractionDelay = 10.f;

	UPROPERTY()
	FTimerHandle InteractionDelay_TimerHandle;

	UPROPERTY(ReplicatedUsing="OnRep_IsEnabled", EditDefaultsOnly, Category = "Rogue|Pickup|Interaction")
	bool IsEnabled = true;

	UFUNCTION()
	void OnRep_IsEnabled();
};
