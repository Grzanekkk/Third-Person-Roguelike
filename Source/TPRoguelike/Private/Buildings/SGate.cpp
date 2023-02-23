// Made by Jan Puto 2023 :D


#include "Buildings/SGate.h"
#include "Components/SphereComponent.h"
#include "Net/UnrealNetwork.h"

// Sets default values
ASGate::ASGate()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootSphere = CreateDefaultSubobject<USphereComponent>(TEXT("RootSphere"));
	SetRootComponent(RootSphere);

	LeftWingColumn = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LeftWingColumn"));
	LeftWingColumn->SetupAttachment(RootComponent);

	RightWingColumn = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RightWingColumn"));
	RightWingColumn->SetupAttachment(RootComponent);

	LeftWingPivot = CreateDefaultSubobject<USphereComponent>(TEXT("LeftWingPivot"));
	LeftWingPivot->SetupAttachment(LeftWingColumn);

	RightWingPivot = CreateDefaultSubobject<USphereComponent>(TEXT("RightWingPivot"));
	RightWingPivot->SetupAttachment(RightWingColumn);

	LeftWingMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LeftWing"));
	LeftWingMesh->SetupAttachment(LeftWingPivot);

	RightWingMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RightWing"));
	RightWingMesh->SetupAttachment(RightWingPivot);

	bIsGateOpened = false;
	bIsGateOpening = false;
	bReplicates = true;
}

void ASGate::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIsGateOpening)
	{
		OpeningGateTick();
	}
}

void ASGate::OpenGate()
{
	bIsGateOpening = true;
}

void ASGate::OpeningGateTick()
{
	if (CurrentGateAngle < OpenedGateAngle)
	{
		CurrentGateAngle += GateOpeningSpeed * GateOpeningDirection * GetWorld()->GetDeltaSeconds();
		FRotator LeftWingsRotation = FRotator(0.0f, -CurrentGateAngle, 0.0f);
		FRotator RigthWingsRotation = FRotator(0.0f, CurrentGateAngle, 0.0f);
		LeftWingPivot->SetRelativeRotation(LeftWingsRotation);
		RightWingPivot->SetRelativeRotation(RigthWingsRotation);
	}
	else
	{
		bIsGateOpening = false;
		bIsGateOpened = true;
	}
}


void ASGate::Interact_Implementation(APawn* InstigatorPawn)
{
	OpenGate();
}

bool ASGate::CanInteract_Implementation(APawn* InstigatorPawn)
{
	return !bIsGateOpened && !bIsGateOpening;
}

bool ASGate::IsEnabled_Implementation()
{
	return true;
}

bool ASGate::CanBeInteractedByPlayerCharacter_Implementation()
{
	return false;
}

void ASGate::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ASGate, bIsGateOpening);
}