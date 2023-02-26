// Made by Jan Puto 2022 :D


#include "ObjectiveArea/CaptureZone.h"
#include "Components/CapsuleComponent.h"
#include "Characters/SCharacter.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Net/UnrealNetwork.h"
#include "FunctionLibrary/GameplayFunctionLibrary.h"


ACaptureZone::ACaptureZone()
{
	FlagPoleMesh = CreateDefaultSubobject<UStaticMeshComponent>("FlagPoleMesh");
	FlagPoleMesh->SetupAttachment(RootComponent);

	FlagMesh = CreateDefaultSubobject<UStaticMeshComponent>("FlagMesh");
	FlagMesh->SetupAttachment(RootComponent);

	QuestIndicatorMesh = CreateDefaultSubobject<UStaticMeshComponent>("QuestIndicator");
	QuestIndicatorMesh->SetupAttachment(RootComponent);
	QuestIndicatorMesh->SetHiddenInGame(true);

	bReplicates = true;
}

void ACaptureZone::BeginPlay()
{
	Super::BeginPlay();
}

void ACaptureZone::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (UKismetSystemLibrary::IsServer(GetWorld()))
	{
		if (IsBeingCaptured())
		{
			CalculateCapturePoints();
		}
	}
}

void ACaptureZone::CalculateCapturePoints()
{
	// This runs only on the server
	CurrentCapPoints = FMath::Clamp(CurrentCapPoints + CapPointPerTick * GetCaptureSpeedMultiplier() * GetWorld()->DeltaTimeSeconds, 0, MaxCapPoints);

	// We need to call it directly on the server
	OnRep_CurrentCapPoints();
}

float ACaptureZone::GetCaptureSpeedMultiplier()
{
	if (PlayersInsideZone.Num() > CapPointPerPlayerMultiplier.Num())
	{
		return CapPointPerPlayerMultiplier.Last();
	}

	return CapPointPerPlayerMultiplier[PlayersInsideZone.Num()];
}

bool ACaptureZone::IsBeingCaptured()
{
	// If there is at least one player inside the zone its is being captured
	return PlayersInsideZone.Num() > 0;
}

float ACaptureZone::GetZoneCapturePercent()
{
	return (CurrentCapPoints / MaxCapPoints) * 100;
}

void ACaptureZone::SetFlagHight()
{
	FVector FlagLocation = FlagMesh->GetRelativeLocation();
	FlagLocation.Z = UKismetMathLibrary::MapRangeClamped(CurrentCapPoints, 0, MaxCapPoints, 0, FlagTargetHight);

	FlagMesh->SetRelativeLocation(FlagLocation);
}

void ACaptureZone::OnZoneCaptured()
{
	OnZoneCapturedEvent.Broadcast(this, PlayersInsideZone);

	UGameplayFunctionLibrary::AddObjectiveStat(GetWorld(), ObjectiveTag, 1);
}

void ACaptureZone::OnRep_CurrentCapPoints()
{
	// update progress bar of smth (or some visual state)
	if (!bIsCaptured && CurrentCapPoints >= MaxCapPoints)
	{
		bIsCaptured = true;
		QuestIndicatorMesh->SetHiddenInGame(false);
		OnZoneCaptured();
	}

	SetFlagHight();
}

void ACaptureZone::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ACaptureZone, CurrentCapPoints);
}