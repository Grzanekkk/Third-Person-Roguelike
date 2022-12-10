// Made by Jan Puto 2022 :D


#include "GameModes/SGameModeBase.h"
#include "EnvironmentQuery/EnvQueryManager.h"
#include "EnvironmentQuery/EnvQueryInstanceBlueprintWrapper.h"
#include "AI/SAICharacter.h"
#include "Characters/SCharacter.h"
#include "Components/SAttributeComponent.h"
#include "PlayerStates/SPlayerState.h"
#include "EngineUtils.h"
#include "DrawDebugHelpers.h"

static TAutoConsoleVariable<bool> CVarSpawnBots(TEXT("jp.SpawnBots"), true, TEXT("Enable spawning of bots via timer."), ECVF_Cheat);
static TAutoConsoleVariable<bool> CVarSpawnPickups(TEXT("jp.SpawnPickups"), true, TEXT("Enable spawning of bots via timer."), ECVF_Cheat);


ASGameModeBase::ASGameModeBase()
{

}

void ASGameModeBase::StartPlay()
{
	Super::StartPlay();

	GetWorldTimerManager().SetTimer(TimerHandle_SpawnBot, this, &ASGameModeBase::SpawnBotTimerElapsed, SpawnTimerInterval, true);

	SpawnPickups();
}


////////////////////////////////////////////////////
/// Bots Spawning
void ASGameModeBase::SpawnBotTimerElapsed()
{
	if (!bCanSpawnBots)
	{
		UE_LOG(LogTemp, Warning, TEXT("Spawning bots disabled via bCanSpawnBots in GameMode"));
		return;
	}

	if (!CVarSpawnBots.GetValueOnGameThread())
	{
		UE_LOG(LogTemp, Warning, TEXT("Spawning bots disabled via CVarSpawnBots command"));
		return;
	}

	// Basicly GetAllActorsOfClass
	int32 NrOfAliveBots = 0;
	for (TActorIterator<ASAICharacter> Iterator(GetWorld()); Iterator; ++Iterator)
	{
		TObjectPtr<ASAICharacter> Bot = *Iterator;

		if (USAttributeComponent::IsActorAlive(Bot))
		{
			NrOfAliveBots++;
		}
	}

	if (ensureMsgf(DifficultyCurve, TEXT("DifficultyCurve is missing! Please assign DifficultyCurve in your Game Mode")))
	{
		MaxBotCount = DifficultyCurve->GetFloatValue(GetWorld()->TimeSeconds);
		UE_LOG(LogTemp, Warning, TEXT("Bots on map: %i, Bots limit: %i"), NrOfAliveBots, MaxBotCount);
	}

	// Checks if we can spawn more bots based on the amount of them in the level
	if (NrOfAliveBots >= MaxBotCount)
	{
		UE_LOG(LogTemp, Warning, TEXT("Max amount of bots on map. Skipping bot spawn"));
		return;
	}

	// RandomBest5Pct mean that as a result we will get SINGLE random locatoin from bast 5 percent results
	UEnvQueryInstanceBlueprintWrapper* QueryInstance = UEnvQueryManager::RunEQSQuery(this, SpawnBotQuery, this, EEnvQueryRunMode::RandomBest5Pct, nullptr);

	if (ensure(QueryInstance))
	{
		QueryInstance->GetOnQueryFinishedEvent().AddDynamic(this, &ASGameModeBase::OnSpawnBotQueryCompleted);
	}
}

void ASGameModeBase::OnSpawnBotQueryCompleted(UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus)
{
	if (QueryStatus != EEnvQueryStatus::Success)
	{
		UE_LOG(LogTemp, Error, TEXT("Spawn Bot EQS failed!"));
		return;
	}
	
	TArray<FVector> Locations = QueryInstance->GetResultsAsLocations();
	if (Locations.Num() > 0)
	{
		GetWorld()->SpawnActor<AActor>(MinionClass, Locations[0], FRotator::ZeroRotator);
		DrawDebugSphere(GetWorld(), Locations[0], 20, 20, FColor::Blue, false, 20.f);
	}
}


////////////////////////////////////////////////////
/// Player Respawning
void ASGameModeBase::RespawnPlayerElapsed(AController* PlayerController)
{
	UE_LOG(LogTemp, Warning, TEXT("Respawning Player"));

	PlayerController->UnPossess();
	RestartPlayer(PlayerController);
}


////////////////////////////////////////////////////
/// Pickup spawning
void ASGameModeBase::SpawnPickups()
{
	if (!bCanSpawnPickups)
	{
		UE_LOG(LogTemp, Warning, TEXT("Spawning bots disabled via bCanSpawnBots in GameMode"));
		return;
	}

	if (!CVarSpawnPickups.GetValueOnGameThread())
	{
		UE_LOG(LogTemp, Warning, TEXT("Spawning bots disabled via CVarSpawnBots command"));
		return;
	}

	UEnvQueryInstanceBlueprintWrapper* QueryInstance = UEnvQueryManager::RunEQSQuery(this, SpawnPickupsQuery, this, EEnvQueryRunMode::AllMatching, nullptr);

	if (ensure(QueryInstance))
	{
		QueryInstance->GetOnQueryFinishedEvent().AddDynamic(this, &ASGameModeBase::OnSpawnQueryCompleted);
	}
}

void ASGameModeBase::OnSpawnQueryCompleted(UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus)
{
	if (QueryStatus != EEnvQueryStatus::Success)
	{
		UE_LOG(LogTemp, Error, TEXT("Spawn Pickups EQS failed!"));
		return;
	}

	if (PickUpsAvalibleToSpawn.Num() == 0)
	{
		UE_LOG(LogTemp, Error, TEXT("PickUpsAvalibleToSpawn is Empty! Failed to Spawn Any Pickups"));
		return;
	}

	TArray<FVector> Locations = QueryInstance->GetResultsAsLocations();
	for (FVector SpawnLocation : Locations)
	{
		float Rand = FMath::RandRange(0, 100);
		if (Rand < PickupSpawnChancePercentage)
		{
			int32 RandPickup = FMath::RandRange(0, PickUpsAvalibleToSpawn.Num() - 1);
			int32 RandZRotation = FMath::RandRange(0, 360);

			GetWorld()->SpawnActor<AActor>(PickUpsAvalibleToSpawn[RandPickup], SpawnLocation, FRotator(0, RandZRotation, 0));
		}
	}
}


////////////////////////////////////////////////////
/// OnActorKilled
void ASGameModeBase::OnActorKilled(AActor* VictimActor, AActor* Killer)
{
	TObjectPtr<ASCharacter> PlayerKilled = Cast<ASCharacter>(VictimActor);
	if (PlayerKilled)
	{
		// Its here and not in header file because we do not want to reuse this timer wher respawning two players at the same time
		FTimerHandle RespawnDelay_TimerHandle;
		FTimerDelegate RespawnPlayer_Delegate;

		RespawnPlayer_Delegate.BindUFunction(this, "RespawnPlayerElapsed", PlayerKilled->GetController());

		GetWorldTimerManager().SetTimer(RespawnDelay_TimerHandle, RespawnPlayer_Delegate, PlayerRespawnDelay, false);
	}

	TObjectPtr<ASCharacter> PlayerKiller = Cast<ASCharacter>(Killer);
	if (PlayerKiller)
	{
		TObjectPtr<USAttributeComponent> VictimAttribComp = Cast<USAttributeComponent>(VictimActor->GetComponentByClass(USAttributeComponent::StaticClass()));
		if (VictimAttribComp)
		{
			PlayerKiller->GetPlayerState<ASPlayerState>()->AddCredits(VictimAttribComp->GetCreditsAmountForKill());
		}
	}

	UE_LOG(LogTemp, Warning, TEXT("OnActorKilled: Victim: %s, Killer: %s"), *GetNameSafe(VictimActor), *GetNameSafe(VictimActor));
}


////////////////////////////////////////////////////
/// Cheats
void ASGameModeBase::KillAllAI()
{
	for (TActorIterator<ASAICharacter> Iterator(GetWorld()); Iterator; ++Iterator)
	{
		TObjectPtr<ASAICharacter> Bot = *Iterator;

		USAttributeComponent::GetAttributeComponent(Bot)->Kill(Bot);
	}
}