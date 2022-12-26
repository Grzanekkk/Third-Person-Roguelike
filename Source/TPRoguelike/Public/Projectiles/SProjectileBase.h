// Made by Jan Puto 2022 :D

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SProjectileBase.generated.h"

class UProjectileMovementComponent;
class USphereComponent;
class USoundCue;

UCLASS(Abstract) // We cant spawn this actor + more... *
class TPROGUELIKE_API ASProjectileBase : public AActor
{
	GENERATED_BODY()

public:
	ASProjectileBase();

	UPROPERTY(EditDefaultsOnly, Category = "Rogue|Effects|Particles")
	TObjectPtr<UParticleSystem> ImpactVFX;

	UPROPERTY(EditDefaultsOnly, Category = "Rogue|Effects|Particles")
	TObjectPtr<UParticleSystem> SpawnVFX;	// Muzzle Flash

	UPROPERTY(EditDefaultsOnly, Category = "Rogue|Effects|Speed")
	float ProjectileSpeed = 3000.f;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Rogue|Components")
	TObjectPtr<UParticleSystemComponent> ParticleComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Rogue|Components")
	TObjectPtr<USphereComponent> SphereComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Rogue|Components")
	TObjectPtr<UProjectileMovementComponent> MovementComp;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Rogue|Components")
	TObjectPtr<UAudioComponent> AudioComp;

	UPROPERTY(EditDefaultsOnly, Category = "Rogue|Effects|Camera Shake")
	TSubclassOf<UCameraShakeBase> ImpactCameraShake;

	UFUNCTION()
	virtual void OnComponentHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	UFUNCTION()
	virtual void OnActorOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	UFUNCTION(BlueprintCallable, Category = "Rogue|Damage")
	virtual void Explode();

	virtual void BeginPlay() override;

	virtual void PostInitializeComponents() override;


	////////////////////////////////////////////////////
	/// Sounds
	UPROPERTY(EditDefaultsOnly, Category = "Rogue|Effects|Sounds")
	TObjectPtr<USoundCue> FlightSound;

	UPROPERTY(EditDefaultsOnly, Category = "Rogue|Effects|Sounds")
	TObjectPtr<USoundCue> ImpactSound;
};
