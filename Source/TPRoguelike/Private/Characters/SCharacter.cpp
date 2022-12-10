 // Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/SCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/SInteractionComponent.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/SAttributeComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SActionComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"

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


//////////////////////////////////////////////////////
//////	Health + Death
void ASCharacter::OnHealthChanged(AActor* InstigatorActor, USAttributeComponent* OwningComp, float NewHealth,
	float DeltaHealth)
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

	PlayerInputComponent->BindAction("PrimaryAttack", IE_Pressed, this, &ASCharacter::PrimaryAttack);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ASCharacter::Jump);
	PlayerInputComponent->BindAction("PrimaryInteract", IE_Pressed, this, &ASCharacter::PrimaryInteract);
	PlayerInputComponent->BindAction("Q_Ability", IE_Pressed, this, &ASCharacter::Q_Ability);
	PlayerInputComponent->BindAction("E_Ability", IE_Pressed, this, &ASCharacter::E_Ability);
	PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &ASCharacter::StartSprint);
	PlayerInputComponent->BindAction("Sprint", IE_Released, this, &ASCharacter::StopSprint);
	PlayerInputComponent->BindAction("Parry", IE_Pressed, this, &ASCharacter::StartParry);
	PlayerInputComponent->BindAction("Parry", IE_Released, this, &ASCharacter::StopParry);
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

void ASCharacter::StartSprint()
{
	ActionComponent->StartActionByName(this, "Sprint");
}

void ASCharacter::StopSprint()
{
	ActionComponent->StopActionByName(this, "Sprint");
}

void ASCharacter::Jump()
{
	ACharacter::Jump();
}


//////////////////////////////////////////////////////
//////	Abilities
void ASCharacter::PrimaryAttack()
{
	ActionComponent->StartActionByName(this, "PrimaryAttack");
}

void ASCharacter::Q_Ability()
{
	ActionComponent->StartActionByName(this, "QAbility");
}

void ASCharacter::E_Ability()
{
	ActionComponent->StartActionByName(this, "EAbility");
}

void ASCharacter::StartParry()
{
	ActionComponent->StartActionByName(this, "Parry");
}

void ASCharacter::StopParry()
{
	ActionComponent->StopActionByName(this, "Parry");
}

void ASCharacter::PrimaryInteract()
{
	InteractionComponent->PrimaryInteract();
}


////////////////////////////////////////////////////
/// Cheats
void ASCharacter::HealSelf(float Amount /* = 100.f */)
{
	AttributeComponent->ApplyHealthChange(this, Amount);
}