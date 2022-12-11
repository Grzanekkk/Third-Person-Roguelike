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

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<USAttributeComponent> AttributeComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<USActionComponent> ActionComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UPawnSensingComponent> PawnSensingComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UParticleSystemComponent> HealingParticleComp;

	UFUNCTION()
	void OnPawnSeen(APawn* Pawn);

	UFUNCTION()
	void RemovePlayerSpottedWidget();

	UFUNCTION()
	void SpawnPlayerSpottedWidget();

	////////////////////////////////////////////////////
	/// Health + Death
	UFUNCTION()
	void OnHealthChanged(AActor* InstigatorActor, USAttributeComponent* OwningComp, float NewHealth, float DeltaHealth);

	UFUNCTION()
	void OnDeath();

	UFUNCTION()
	bool SetTargetActor(AActor* TargetActor);

	UPROPERTY(VisibleAnywhere, Category = "RL|Effects")
	FName TimeToHitParamName = "TimeToHit";

	UPROPERTY(VisibleAnywhere, Category = "RL|Healing")
	bool bIsHealingOverTime = false;

	UPROPERTY(VisibleAnywhere, Category = "RL|Healing")
	float HealingInterval = 0.05f;

	UPROPERTY(VisibleAnywhere, Category = "RL|Healing")
	float HealthPerSecond = 0.f;

	UPROPERTY(VisibleAnywhere, Category = "RL|Healing")
	float SecondsOfHealing = 0.f;

	UPROPERTY(VisibleAnywhere, Category = "RL|Healing")
	float TimeSinceLastHealing = 0.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly,  Category = "RL|UI|HealthBar")
	TSubclassOf<USWorldUserWidget> HealthBarWidgetClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "RL|UI|HealthBar")
	TObjectPtr<USWorldUserWidget> HealthBarWidgetInstance;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "RL|UI|PlayerSpotted")
	TSubclassOf<USPlayerSpottedWidget> PlayerSpottedWidgetClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "RL|UI|PlayerSpotted")
	TObjectPtr<USPlayerSpottedWidget> PlayerSpottedWidgetInstance;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "RL|UI|PlayerSpotted")
	float PlayerSpottedWidgetLifeTime = 2.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "RL|UI|PlayerSpotted")
	FTimerHandle PlayerSpottedTimer;

	/// BlackBoard Values
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "RL|UI|PlayerSpotted")
	FName BBV_TargetActor = "TargetActor";



public:
	UFUNCTION()
	void Heal(float HealingAmount);

	UFUNCTION()
	void StartHealingOverTime(float _HealthPerSecond, float _HealForSeconds);

	UFUNCTION()
	void StopHealingOverTime();


protected:
	virtual void PostInitializeComponents() override;
};
