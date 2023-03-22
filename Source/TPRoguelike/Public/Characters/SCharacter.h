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
	UPROPERTY(EditDefaultsOnly, Category = "Rogue|Components")
	TObjectPtr<UCameraComponent> CameraComp;

	UPROPERTY(EditDefaultsOnly, Category = "Rogue|Components")
	TObjectPtr<USpringArmComponent> SpringArmComp;

	UPROPERTY(EditDefaultsOnly, Category = "Rogue|Components")
	TObjectPtr<USInteractionComponent> InteractionComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Rogue|Components")
	TObjectPtr<USAttributeComponent> AttributeComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Rogue|Components")
	TObjectPtr<USActionComponent> ActionComponent;

	////////////////////////////////////////////////////
	/// Movement
	UFUNCTION()
	void MoveForward(float Value);

	UFUNCTION()
	void MoveRight(float Value);

	////////////////////////////////////////////////////
	/// Interaction
	UFUNCTION()
	void F_Action_Start();
	UFUNCTION()
	void F_Action_Stop();
	
	////////////////////////////////////////////////////
	/// Abilities
	UFUNCTION()
	void LMB_Action_Start();
	UFUNCTION()
	void LMB_Action_Stop();

	UFUNCTION()
	void Q_Ability_Start();
	UFUNCTION()
	void Q_Ability_Stop();

	UFUNCTION()
	void E_Ability_Start();
	UFUNCTION()
	void E_Ability_Stop();

	UFUNCTION()
	void X_Action_Start();
	UFUNCTION()
	void X_Action_Stop();

	UFUNCTION()
	void RMB_Action_Start();
	UFUNCTION()
	void RMB_Action_Stop();

	UFUNCTION()
	void Shift_Action_Start();
	UFUNCTION()
	void Shift_Action_Stop();

	UFUNCTION()
	void Space_Action_Start();
	UFUNCTION()
	void Space_Action_Stop();

	UFUNCTION()
	void Control_Action_Start();
	UFUNCTION()
	void Control_Action_Stop();

	////////////////////////////////////////////////////
	/// Health + Death
	UFUNCTION()
	void OnHealthChanged(AActor* InstigatorActor, USAttributeComponent* OwningComp, float NewHealth, float DeltaHealth);

	UFUNCTION()
	void OnDeath();

	UPROPERTY(VisibleAnywhere, Category = "Rogue|Effects")
	FName TimeToHitParamName = "TimeToHit";

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Rogue|Debug")
	bool bDrawDebugPlayerCameraRotationArrows = false;

public:	
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	////////////////////////////////////////////////////
	/// Cheats
	UFUNCTION(Exec)
	void HealSelf(float Amount = 100.f);
};
