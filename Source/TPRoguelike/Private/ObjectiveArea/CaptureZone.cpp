// Made by Jan Puto 2022 :D


#include "ObjectiveArea/CaptureZone.h"
#include "Components/CapsuleComponent.h"
#include "Characters/SCharacter.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "Net/UnrealNetwork.h"
#include "Kismet/GameplayStatics.h"
#include "Components/SQuestManagerComponent.h"
#include "GameState/SGameState.h"


ACaptureZone::ACaptureZone()
{
	TriggerBox = CreateDefaultSubobject<UCapsuleComponent>("Zone Area");
	RootComponent = TriggerBox;

	AreaIndicatorMesh = CreateDefaultSubobject<UStaticMeshComponent>("AreaIndicatorMesh");
	AreaIndicatorMesh->SetupAttachment(RootComponent);

	FlagPoleMesh = CreateDefaultSubobject<UStaticMeshComponent>("FlagPoleMesh");
	FlagPoleMesh->SetupAttachment(RootComponent);

	FlagMesh = CreateDefaultSubobject<UStaticMeshComponent>("FlagMesh");
	FlagMesh->SetupAttachment(RootComponent);

	QuestIndicatorMesh = CreateDefaultSubobject<UStaticMeshComponent>("QuestIndicator");
	QuestIndicatorMesh->SetupAttachment(RootComponent);
	QuestIndicatorMesh->SetHiddenInGame(true);

	bReplicates = true;
}

void ACaptureZone::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	TriggerBox->OnComponentBeginOverlap.AddDynamic(this, &ACaptureZone::StartOverlapingZone);
	TriggerBox->OnComponentEndOverlap.AddDynamic(this, &ACaptureZone::StopOverlapingZone);
}

void ACaptureZone::BeginPlay()
{
	Super::BeginPlay();

	//if (UKismetSystemLibrary::IsServer(GetWorld()))
	//{
	//	TObjectPtr<ASGameState> GameState = Cast<ASGameState>(UGameplayStatics::GetGameState(GetWorld()));
	//	if (GameState)
	//	{
	//		GameState->ServerOnlyAddCaptureZoneToActiveList(this);
	//	}
	//}
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


void ACaptureZone::StartOverlapingZone(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (UKismetSystemLibrary::IsServer(GetWorld()))
	{
		TObjectPtr<ASCharacter> PlayerCharacter = Cast<ASCharacter>(OtherActor);
		if (PlayerCharacter && !PlayersInsideZone.Contains(PlayerCharacter))
		{
			// We know new player entered the zone
			PlayersInsideZone.Add(PlayerCharacter);
		}
	}
}


void ACaptureZone::StopOverlapingZone(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (UKismetSystemLibrary::IsServer(GetWorld()))
	{
		TObjectPtr<ASCharacter> PlayerCharacter = Cast<ASCharacter>(OtherActor);
		if (PlayerCharacter && PlayersInsideZone.Contains(PlayerCharacter))
		{
			// We know player left the zone
			PlayersInsideZone.Remove(PlayerCharacter);
		}
	}
}


void ACaptureZone::CalculateCapturePoints()
{
	// This runs only on the server
	CurrentCapPoints = FMath::Clamp(CurrentCapPoints + CapPointPerTick * GetCaptureSpeedMultiplier() * GetWorld()->DeltaTimeSeconds, 0, MaxCapPoints);

	// We need to call it directly on the server
	OnRep_CurrentCapPoints();

	//if (!bIsCaptured)
	//{
	//	if (CurrentCapPoints >= MaxCapPoints)
	//	{
	//		// We just captured the zone
	//		bIsCaptured = true;
	//	}
	//}
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

	// For Quests
	if (UKismetSystemLibrary::IsServer(GetWorld()))
	{
		TObjectPtr<ASGameState> GameState = Cast<ASGameState>(UGameplayStatics::GetGameState(GetWorld()));
		if (GameState)
		{
			TObjectPtr<USQuestManagerComponent> QuestManager = GameState->GetQuestManager();
			if (QuestManager)
			{
				QuestManager->ServerOnlyAddObjectiveStat(ObjectiveTag, 1);
			}
		}
	}
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