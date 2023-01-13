// Made by Jan Puto 2022 :D

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SAICharacter.generated.h"

class USAttributeComponent;
class USActionComponent;
class UPawnSensingComponent;
class USWorldUserWidget;
class USPlayerSpottedWidget;

UCLASS()
class TPROGUELIKE_API ASAICharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ASAICharacter();

	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void Heal(float HealingAmount);

	UFUNCTION()
	void StartHealingOverTime(float _HealthPerSecond, float _HealForSeconds);

	UFUNCTION()
	void StopHealingOverTime();

protected:

	virtual void PostInitializeComponents() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Rogue|Components")
	TObjectPtr<USAttributeComponent> AttributeComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Rogue|Components")
	TObjectPtr<USActionComponent> ActionComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Rogue|Components")
	TObjectPtr<UPawnSensingComponent> PawnSensingComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Rogue|Components")
	TObjectPtr<UParticleSystemComponent> HealingParticleComp;

	UFUNCTION()
	void OnPawnSeen(APawn* Pawn);

	UFUNCTION()
	void RemovePlayerSpottedWidget();

	UFUNCTION(NetMulticast, Reliable)
	void MulticastSpawnPlayerSpottedWidget();

	////////////////////////////////////////////////////
	/// Health + Death
	UFUNCTION()
	void OnHealthChanged(AActor* InstigatorActor, USAttributeComponent* OwningComp, float NewHealth, float DeltaHealth);

	UFUNCTION()
	void OnDeath();

	UFUNCTION()
	bool SetTargetActor(AActor* TargetActor);

	UPROPERTY(VisibleAnywhere, Category = "Rogue|Effects")
	FName TimeToHitParamName = "TimeToHit";

	UPROPERTY(VisibleAnywhere, Category = "Rogue|Healing")
	bool bIsHealingOverTime = false;

	UPROPERTY(VisibleAnywhere, Category = "Rogue|Healing")
	float HealingInterval = 0.05f;

	UPROPERTY(VisibleAnywhere, Category = "Rogue|Healing")
	float HealthPerSecond = 0.f;

	UPROPERTY(VisibleAnywhere, Category = "Rogue|Healing")
	float SecondsOfHealing = 0.f;

	UPROPERTY(VisibleAnywhere, Category = "Rogue|Healing")
	float TimeSinceLastHealing = 0.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly,  Category = "Rogue|UI|HealthBar")
	TSubclassOf<USWorldUserWidget> HealthBarWidgetClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Rogue|UI|HealthBar")
	TObjectPtr<USWorldUserWidget> HealthBarWidgetInstance;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Rogue|UI|PlayerSpotted")
	TSubclassOf<USPlayerSpottedWidget> PlayerSpottedWidgetClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Rogue|UI|PlayerSpotted")
	TObjectPtr<USPlayerSpottedWidget> PlayerSpottedWidgetInstance;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Rogue|UI|PlayerSpotted")
	float PlayerSpottedWidgetLifeTime = 2.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Rogue|UI|PlayerSpotted")
	FTimerHandle PlayerSpottedTimer;

	/// BlackBoard Values
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Rogue|UI|PlayerSpotted")
	FName BBV_TargetActor = "TargetActor";

};
