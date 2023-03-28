 // Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/SCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SActionComponent.h"
#include "Components/SAttributeComponent.h"
#include "Components/SInteractionComponent.h"
#include "Components/RogueInputComponent.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

//////////////////////////////////////////////////////
//////	Setups
ASCharacter::ASCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArmComp->bUsePawnControlRotation = true;
	SpringArmComp->SetupAttachment(RootComponent);

	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	CameraComp->SetupAttachment(SpringArmComp);

	InteractionComponent = CreateDefaultSubobject<USInteractionComponent>(TEXT("InteractionComponent"));

	AttributeComponent = CreateDefaultSubobject<USAttributeComponent>(TEXT("AttributeComponent"));

	ActionComponent = CreateDefaultSubobject<USActionComponent>(TEXT("ActionComponent"));

	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;

	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Ignore);
	GetMesh()->SetGenerateOverlapEvents(true);

	// AI
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}

void ASCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	AttributeComponent->OnHealthChanged.AddDynamic(this, &ASCharacter::OnHealthChanged);
}


void ASCharacter::BeginPlay()
{
	Super::BeginPlay();

	SetupPlayerEnhancedInput();
}

void ASCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bDrawDebugPlayerCameraRotationArrows)
	{
		// -- Rotation Visualization -- //
		const float DrawScale = 100.0f;
		const float Thickness = 5.0f;

		FVector LineStart = GetActorLocation();
		// Offset to the right of pawn
		LineStart += GetActorRightVector() * 100.0f;
		// Set line end in direction of the actor's forward
		FVector ActorDirection_LineEnd = LineStart + (GetActorForwardVector() * 100.0f);
		// Draw Actor's Direction
		DrawDebugDirectionalArrow(GetWorld(), LineStart, ActorDirection_LineEnd, DrawScale, FColor::Yellow, false, 0.0f, 0, Thickness);

		FVector ControllerDirection_LineEnd = LineStart + (GetControlRotation().Vector() * 100.0f);
		// Draw 'Controller' Rotation ('PlayerController' that 'possessed' this character)
		DrawDebugDirectionalArrow(GetWorld(), LineStart, ControllerDirection_LineEnd, DrawScale, FColor::Green, false, 0.0f, 0, Thickness);
	}
}

void ASCharacter::SetupPlayerEnhancedInput()
{

}


//////////////////////////////////////////////////////
//////	Health + Death
void ASCharacter::OnHealthChanged(AActor* InstigatorActor, USAttributeComponent* OwningComp, float NewHealth, float DeltaHealth)
{
	if (DeltaHealth < 0.0f)
	{
		GetMesh()->SetScalarParameterValueOnMaterials(TimeToHitParamName, GetWorld()->GetTimeSeconds());
	}

	if (DeltaHealth < 0 && !AttributeComponent->IsAlive())
	{
		OnDeath();
	}
}

void ASCharacter::OnDeath()
{
	TObjectPtr<APlayerController> PlayerControler = Cast<APlayerController>(Controller);
	DisableInput(PlayerControler);

	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}


//////////////////////////////////////////////////////
//////	Movement + interactions
void ASCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	URogueInputComponent* RogueIC = CastChecked<URogueInputComponent>(PlayerInputComponent);
	if (RogueIC)
	{
		RogueIC->SetMappingContext(Controller, DefaultCharacterInputMapping, 0);

		TArray<uint32> BindHandles;
		//RogueIC->BindAbilityActions(CharacterInputConfig, this, &ThisClass::Input_AbilityInputTagPressed, &ThisClass::Input_AbilityInputTagReleased, /*out*/ BindHandles);

		//RogueIC->BindNativeAction(CharacterInputConfig, MoveTag, ETriggerEvent::Triggered, this, &ThisClass::Input_Move);
		//RogueIC->BindNativeAction(DefaultCharacterInputMapping, LookTag, ETriggerEvent::Triggered, this, &ASCharacter::Input_LookMouse, /*bLogIfNotFound=*/ false);
		//RogueIC->BindNativeAction(DefaultCharacterInputMapping, JumpTag, ETriggerEvent::Triggered, this, &ASCharacter::Input_Jump_Start, /*bLogIfNotFound=*/ false);
		//RogueIC->BindNativeAction(DefaultCharacterInputMapping, SprintTag, ETriggerEvent::Triggered, this, &ASCharacter::Input_Sprint_Start, /*bLogIfNotFound=*/ false);
		//RogueIC->BindNativeAction(DefaultCharacterInputMapping, SprintTag, ETriggerEvent::Canceled, this, &ASCharacter::Input_Sprint_Stop, /*bLogIfNotFound=*/ false);
		//RogueIC->BindNativeAction(DefaultCharacterInputMapping, CrouchTag, ETriggerEvent::Triggered, this, &ASCharacter::Crouch_Start, /*bLogIfNotFound=*/ false);
	}
}

void ASCharacter::Input_Move(const FInputActionValue& InputActionValue)
{
	if (Controller)
	{
		const FVector2D Value = InputActionValue.Get<FVector2D>();
		const FRotator MovementRotation(0.0f, Controller->GetControlRotation().Yaw, 0.0f);

		if (Value.X != 0.0f)
		{
			const FVector MovementDirection = MovementRotation.RotateVector(FVector::RightVector);
			this->AddMovementInput(MovementDirection, Value.X);
		}

		if (Value.Y != 0.0f)
		{
			const FVector MovementDirection = MovementRotation.RotateVector(FVector::ForwardVector);
			this->AddMovementInput(MovementDirection, Value.Y);
		}
	}
}

void ASCharacter::Input_LookMouse(const FInputActionValue& InputActionValue)
{
	const FVector2D Value = InputActionValue.Get<FVector2D>();

	if (Value.X != 0.0f)
	{
		this->AddControllerYawInput(Value.X);
	}

	if (Value.Y != 0.0f)
	{
		this->AddControllerPitchInput(Value.Y);
	}
}

//////////////////////////////////////////////////////
//////	Actions

void ASCharacter::LMB_Action_Start()
{
	ActionComponent->StartActionByName(this, "PrimaryAttack");
}

void ASCharacter::LMB_Action_Stop()
{
}

void ASCharacter::RMB_Action_Start()
{
	ActionComponent->StartActionByName(this, "Parry");
}

void ASCharacter::RMB_Action_Stop()
{
	ActionComponent->StopActionByName(this, "Parry");
}

void ASCharacter::Input_Sprint_Start(const FInputActionValue& InputActionValue)
{
	ActionComponent->StartActionByName(this, "Sprint");
}

void ASCharacter::Input_Sprint_Stop(const FInputActionValue& InputActionValue)
{
	ActionComponent->StopActionByName(this, "Sprint");
}

void ASCharacter::Input_Jump_Start(const FInputActionValue& InputActionValue)
{
	ACharacter::Jump();
}

void ASCharacter::Input_Jump_Stop(const FInputActionValue& InputActionValue)
{
}

void ASCharacter::Crouch_Start()
{
}

void ASCharacter::Croch_Stop()
{
}

void ASCharacter::Q_Ability_Start()
{
	ActionComponent->StartActionByName(this, "QAbility");
}

void ASCharacter::Q_Ability_Stop()
{
}

void ASCharacter::E_Ability_Start()
{
	ActionComponent->StartActionByName(this, "EAbility");
}

void ASCharacter::E_Ability_Stop()
{
}

void ASCharacter::X_Action_Start()
{
}

void ASCharacter::X_Action_Stop()
{
}

void ASCharacter::F_Action_Start()
{
	InteractionComponent->PrimaryInteract();
}

void ASCharacter::F_Action_Stop()
{
}


////////////////////////////////////////////////////
/// Cheats
void ASCharacter::HealSelf(float Amount /* = 100.f */)
{
	AttributeComponent->ApplyHealthChange(this, Amount);
}