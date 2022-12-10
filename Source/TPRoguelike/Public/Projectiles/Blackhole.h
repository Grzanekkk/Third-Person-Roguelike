// Made by Jan Puto 2022 :D

#pragma once

#include "CoreMinimal.h"
#include "SProjectileBase.h"
#include "Blackhole.generated.h"

UCLASS()
class TPROGUELIKE_API ABlackhole : public ASProjectileBase
{
	GENERATED_BODY()
	
public:	

	ABlackhole();
	
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, Category = "Physics")
	float SuckingStrength = -2000.f;

protected:
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USphereComponent* SuckingSphere;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	class URadialForceComponent* RadialForceComp;
	
	virtual void BeginPlay() override;

	UFUNCTION()
	void StartSucking(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
};
