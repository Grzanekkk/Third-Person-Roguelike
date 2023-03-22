 // Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/SCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SActionComponent.h"
#include "Components/SAttributeComponent.h"
#include "Components/SInteractionComponent.h"
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


//////////////////////////////////////////////////////
//////	BeginPlay + Tick
void ASCharacter::BeginPlay()
{
	Super::BeginPlay();
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

	PlayerInputComponent->BindAxis("MoveForward", this, &ASCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ASCharacter::MoveRight);
	
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);

	PlayerInputComponent->BindAction("LMB_Action", IE_Pressed, this, &ASCharacter::LMB_Action_Start);
	PlayerInputComponent->BindAction("LMB_Action", IE_Released, this, &ASCharacter::LMB_Action_Stop);
	PlayerInputComponent->BindAction("RMB_Action", IE_Pressed, this, &ASCharacter::RMB_Action_Start);
	PlayerInputComponent->BindAction("RMB_Action", IE_Released, this, &ASCharacter::RMB_Action_Stop);
	PlayerInputComponent->BindAction("F_Action", IE_Pressed, this, &ASCharacter::F_Action_Start);
	PlayerInputComponent->BindAction("F_Action", IE_Released, this, &ASCharacter::F_Action_Stop);
	PlayerInputComponent->BindAction("Q_Ability", IE_Pressed, this, &ASCharacter::Q_Ability_Start);
	PlayerInputComponent->BindAction("Q_Ability", IE_Released, this, &ASCharacter::Q_Ability_Stop);
	PlayerInputComponent->BindAction("E_Ability", IE_Pressed, this, &ASCharacter::E_Ability_Start);
	PlayerInputComponent->BindAction("E_Ability", IE_Released, this, &ASCharacter::E_Ability_Stop);
	PlayerInputComponent->BindAction("X_Action", IE_Pressed, this, &ASCharacter::F_Action_Start);
	PlayerInputComponent->BindAction("X_Action", IE_Released, this, &ASCharacter::F_Action_Stop);
	PlayerInputComponent->BindAction("Shift_Action", IE_Pressed, this, &ASCharacter::Shift_Action_Start);
	PlayerInputComponent->BindAction("Shift_Action", IE_Released, this, &ASCharacter::Shift_Action_Stop);
	PlayerInputComponent->BindAction("Space_Action", IE_Pressed, this, &ASCharacter::Space_Action_Start);
	PlayerInputComponent->BindAction("Space_Action", IE_Released, this, &ASCharacter::Space_Action_Stop);
	PlayerInputComponent->BindAction("Control_Action", IE_Pressed, this, &ASCharacter::Control_Action_Start);
	PlayerInputComponent->BindAction("Control_Action", IE_Released, this, &ASCharacter::Control_Action_Start);

}

void ASCharacter::MoveForward(float Value)
{
	FRotator ControlRot = GetControlRotation();
	ControlRot.Pitch = 0.f;
	ControlRot.Roll = 0.f;
	
	AddMovementInput(ControlRot.Vector(), Value);
}

void ASCharacter::MoveRight(float Value)
{
	FRotator ControlRot = GetControlRotation();
	ControlRot.Pitch = 0.f;
	ControlRot.Roll = 0.f;

	FVector RightVector = FRotationMatrix(ControlRot).GetScaledAxis(EAxis::Y);
	
	AddMovementInput(RightVector, Value);
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

void ASCharacter::Shift_Action_Start()
{
	ActionComponent->StartActionByName(this, "Sprint");
}

void ASCharacter::Shift_Action_Stop()
{
	ActionComponent->StopActionByName(this, "Sprint");
}

void ASCharacter::Space_Action_Start()
{
	ACharacter::Jump();
}

void ASCharacter::Space_Action_Stop()
{
}

void ASCharacter::Control_Action_Start()
{
}

void ASCharacter::Control_Action_Stop()
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