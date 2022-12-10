// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SCharacter.generated.h"

class UCameraComponent;
class USpringArmComponent;
class USInteractionComponent;
class USAttributeComponent;
class USActionComponent;
class ASProjectileBase;

UCLASS()
class TPROGUELIKE_API ASCharacter : public ACharacter
{
	GENERATED_BODY()
	
public:
	ASCharacter();

protected:

	virtual void BeginPlay() override;

	virtual void PostInitializeComponents() override;
	

	////////////////////////////////////////////////////
	/// Components
	UPROPERTY(EditDefaultsOnly, Category = "Components")
	TObjectPtr<UCameraComponent> CameraComp;

	UPROPERTY(EditDefaultsOnly, Category = "Components")
	TObjectPtr<USpringArmComponent> SpringArmComp;

	UPROPERTY(EditDefaultsOnly, Category = "Components")
	TObjectPtr<USInteractionComponent> InteractionComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components")
	TObjectPtr<USAttributeComponent> AttributeComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components")
	TObjectPtr<USActionComponent> ActionComponent;


	////////////////////////////////////////////////////
	/// Movement
	UFUNCTION()
	void MoveForward(float Value);

	UFUNCTION()
	void MoveRight(float Value);
	
	UFUNCTION()
	void StartSprint();

	UFUNCTION()
	void StopSprint();

	void Jump();

	////////////////////////////////////////////////////
	/// Interaction
	UFUNCTION()
	void PrimaryInteract();

	
	////////////////////////////////////////////////////
	/// Abilities
	UFUNCTION()
	void PrimaryAttack();

	UFUNCTION()
	void Q_Ability();

	UFUNCTION()
	void E_Ability();

	UFUNCTION()
	void StartParry();
	
	UFUNCTION()
	void StopParry();


	////////////////////////////////////////////////////
	/// Health + Death
	UFUNCTION()
	void OnHealthChanged(AActor* InstigatorActor, USAttributeComponent* OwningComp, float NewHealth, float DeltaHealth);

	UFUNCTION()
	void OnDeath();

	UPROPERTY(VisibleAnywhere, Category = "Effects")
	FName TimeToHitParamName = "TimeToHit";

public:	
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


	////////////////////////////////////////////////////
	/// Cheats
	UFUNCTION(Exec)
	void HealSelf(float Amount = 100.f);
};
