// Made by Jan Puto 2022 :D

#pragma once

#include "CoreMinimal.h"
#include "SProjectileBase.h"
#include "SDashProjectile.generated.h"

UCLASS()
class TPROGUELIKE_API ASDashProjectile : public ASProjectileBase
{
	GENERATED_BODY()
	
public:
	// Sets default values for this actor's properties
	ASDashProjectile();

	UPROPERTY(EditDefaultsOnly, Category = "Effects|Particles")
	TObjectPtr<UParticleSystem> PlayerTeleportVFX;

	UPROPERTY(EditDefaultsOnly, Category = "Dash")
	float ExplodeAfter = 0.4f;

protected:

	virtual void BeginPlay() override;

	virtual void PostInitializeComponents() override;

	virtual void OnComponentHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit) override;

	virtual void OnActorOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

	virtual void Explode() override;

	void TeleportPlayer();

	UPROPERTY(EditDefaultsOnly, Category = "Dash")
	float TeleportDelay = 0.2f;

	UPROPERTY(VisibleAnywhere, Category = "Dash")
	TObjectPtr<AActor> TargetActor = nullptr;

	UPROPERTY()
	FTimerHandle ExpolsionTimer;

	UPROPERTY()
	FTimerHandle TeleportationTimer;
};

