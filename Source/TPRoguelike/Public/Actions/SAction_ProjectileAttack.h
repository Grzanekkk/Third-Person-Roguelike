// Made by Jan Puto 2022 :D

#pragma once

#include "CoreMinimal.h"
#include "Actions/SAction.h"
#include "SAction_ProjectileAttack.generated.h"

class ASProjectileBase;

UCLASS()
class TPROGUELIKE_API USAction_ProjectileAttack : public USAction
{
	GENERATED_BODY()
	
public:

	USAction_ProjectileAttack();

	virtual void StartAction_Implementation(AActor* Instigator) override;

protected:

	UPROPERTY(EditDefaultsOnly, Category = "Attack")
	TSubclassOf<ASProjectileBase> Attack_ProjectileClass;

	UPROPERTY(EditDefaultsOnly, Category = "Attack")
	TObjectPtr<UAnimMontage> AttackAnim;

	UPROPERTY(EditDefaultsOnly, Category = "Attack")
	FName WeaponSocketName;

	UPROPERTY(EditDefaultsOnly, Category = "Attack")
	float AttackAnimDelay;

	UFUNCTION()
	void AtackDelay_Elapsed(ACharacter* InstigatorCharacter);
};
