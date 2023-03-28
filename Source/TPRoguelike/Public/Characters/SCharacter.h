// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GameplayTagContainer.h"
#include "SCharacter.generated.h"

class UCameraComponent;
class USpringArmComponent;
class USInteractionComponent;
class USAttributeComponent;
class USActionComponent;
class ASProjectileBase;
class URogueInputConfig;
class UInputMappingContext;

UCLASS()
class TPROGUELIKE_API ASCharacter : public ACharacter
{
	GENERATED_BODY()
	
public:
	ASCharacter();

	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Rogue|Input")
	UInputMappingContext* DefaultCharacterInputMapping;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Rogue|Input")
	UInputMappingContext* CharacterActionSet;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Rogue|Input")
	URogueInputConfig* CharacterInputConfig;

	////////////////////////////////////////////////////
	/// Cheats
	UFUNCTION(Exec)
	void HealSelf(float Amount = 100.f);

protected:

	virtual void BeginPlay() override;

	virtual void PostInitializeComponents() override;

	void SetupPlayerEnhancedInput();
	
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

	// Tags
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Rogue|Tags")
	FGameplayTag MoveTag;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Rogue|Tags")
	FGameplayTag LookTag;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Rogue|Tags")
	FGameplayTag JumpTag;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Rogue|Tags")
	FGameplayTag SprintTag;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Rogue|Tags")
	FGameplayTag CrouchTag;


	/// Movement
	UFUNCTION()
	void Input_Move(const FInputActionValue& InputActionValue);

	UFUNCTION()
	void Input_LookMouse(const FInputActionValue& InputActionValue);

	/// Interaction
	UFUNCTION()
	void F_Action_Start();
	UFUNCTION()
	void F_Action_Stop();
	
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
	void Input_Sprint_Start(const FInputActionValue& InputActionValue);
	UFUNCTION()
	void Input_Sprint_Stop(const FInputActionValue& InputActionValue);

	UFUNCTION()
	void Input_Jump_Start(const FInputActionValue& InputActionValue);
	UFUNCTION()
	void Input_Jump_Stop(const FInputActionValue& InputActionValue);

	UFUNCTION()
	void Crouch_Start();
	UFUNCTION()
	void Croch_Stop();

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


};
