// Made by Jan Puto 2022 :D

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SGameplayInterface.h"
#include "SPickupBase.generated.h"

class USphereComponent;

UCLASS(Abstract)
class TPROGUELIKE_API ASPickupBase : public AActor, public ISGameplayInterface
{
	GENERATED_BODY()
	
public:	
	ASPickupBase();

	virtual void Tick(float DeltaTime) override;

	virtual void Interact_Implementation(APawn* InstigatorPawn);

	virtual bool CanInteract_Implementation(APawn* InstigatorPawn);

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Pickup")
	int32 UseCost = 0;

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	virtual void UsePickupItem(APawn* InstigatorPawn);

	UFUNCTION()
	void AllowInteraction();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Components")
	TObjectPtr<UStaticMeshComponent> MainMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<USphereComponent> SphereComp;

	UPROPERTY(EditDefaultsOnly, Category = "Effects|Particles")
	TObjectPtr<UParticleSystem> PickUpParticles;

	UPROPERTY(EditDefaultsOnly, Category = "Interaction")
	float InteractionDelay = 10.f;

	UPROPERTY()
	FTimerHandle InteractionDelay_TimerHandle;

	UPROPERTY(EditDefaultsOnly, Category = "Interaction")
	bool bCanBeInteracted = true;
};
