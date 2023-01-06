// Made by Jan Puto 2022 :D


#include "GameModes/SGameModeBase.h"
#include "AI/SAICharacter.h"
#include "Characters/SCharacter.h"
#include "Components/SAttributeComponent.h"
#include "DrawDebugHelpers.h"
#include "EngineUtils.h"
#include "EnvironmentQuery/EnvQueryInstanceBlueprintWrapper.h"
#include "EnvironmentQuery/EnvQueryManager.h"
#include "GameFramework/GameStateBase.h"
#include "Kismet/GameplayStatics.h"
#include "Pickups/SPickupBase.h"
#include "PlayerStates/SPlayerState.h"
#include "SaveSystem/SSaveGame.h"
#include "Serialization/ObjectAndNameAsStringProxyArchive.h"



static TAutoConsoleVariable<bool> CVarSpawnBots(TEXT("jp.SpawnBots"), false, TEXT("Enable spawning of bots via timer."), ECVF_Cheat);
static TAutoConsoleVariable<bool> CVarSpawnPickups(TEXT("jp.SpawnPickups"), true, TEXT("Enable spawning of pickups."), ECVF_Cheat);
static TAutoConsoleVariable<int> CVarGiveCredits(TEXT("jp.GiveCredits"), 0, TEXT("Give credits to every player."), ECVF_Cheat);


ASGameModeBase::ASGameModeBase()
{

}

void ASGameModeBase::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);

	LoadSaveGame();
}

void ASGameModeBase::HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer)
{
	Super::HandleStartingNewPlayer_Implementation(NewPlayer);

	if (NewPlayer)
	{
		TObjectPtr<ASPlayerState> PlayerState = NewPlayer->GetPlayerState<ASPlayerState>();
		if (PlayerState)
		{
			PlayerState->LoadPlayerState(CurrentSaveGame);
		}
	}
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
/// SaveSystem
void ASGameModeBase::WriteSaveGame()
{
	for (int i = 0; i < GameState->PlayerArray.Num(); i++)
	{
		TObjectPtr<ASPlayerState> PlayerState = Cast<ASPlayerState>(GameState->PlayerArray[i]);
		if (PlayerState)
		{
			PlayerState->SavePlayerState(CurrentSaveGame);
			break; // SinglePlayer only for now
		}
	}

	CurrentSaveGame->SavedActors.Empty();

	for (FActorIterator It(GetWorld()); It; ++It)
	{
		TObjectPtr<AActor> Actor = *It;

		if (!Actor->Implements<USGameplayInterface>())
		{
			continue;
		}

		FActorSavedData ActorData;
		ActorData.ActorName = Actor->GetName();
		ActorData.ActorTransform = Actor->GetTransform();

		// Pass the array to fill with binary data
		FMemoryWriter MemWriter(ActorData.ByteData);

		FObjectAndNameAsStringProxyArchive Ar(MemWriter, true);
		// Finds only variables with UPROPERTY(SaveGame)
		Ar.ArIsSaveGame = true;
		// Converts Acrtors SaveGame UPROPERTIES into binary data
		Actor->Serialize(Ar);

		CurrentSaveGame->SavedActors.Add(ActorData);
	}

	UGameplayStatics::SaveGameToSlot(CurrentSaveGame, SaveSlotName, 0);
}

void ASGameModeBase::LoadSaveGame()
{
	if (UGameplayStatics::DoesSaveGameExist(SaveSlotName, 0))
	{
		CurrentSaveGame = Cast<USSaveGame>(UGameplayStatics::LoadGameFromSlot(SaveSlotName, 0));
		if (CurrentSaveGame == nullptr)
		{
			UE_LOG(LogTemp, Error, TEXT("Failed to load SaveGame Data"));
			return;
		}

		UE_LOG(LogTemp, Log, TEXT("Successfully loaded SaveGame Data"));


		for (FActorIterator It(GetWorld()); It; ++It)
		{
			TObjectPtr<AActor> Actor = *It;

			if (!Actor->Implements<USGameplayInterface>())
			{
				continue;
			}

			for (FActorSavedData SavedActorData : CurrentSaveGame->SavedActors)
			{
				if (Actor->GetName() == SavedActorData.ActorName)
				{
					Actor->SetActorTransform(SavedActorData.ActorTransform);

					// Pass array of binary date to read from
					FMemoryReader MemReader(SavedActorData.ByteData);

					FObjectAndNameAsStringProxyArchive Ar(MemReader, true);
					// Finds only variables with UPROPERTY(SaveGame)
					Ar.ArIsSaveGame = true;
					// Converts Acrtors binary data into SaveGame UPROPERTIES
					Actor->Serialize(Ar);

					ISGameplayInterface::Execute_OnActorLoaded(Actor);

					break;
				}
			}
		}
	}
	else
	{
		CurrentSaveGame = Cast<USSaveGame>(UGameplayStatics::CreateSaveGameObject(USSaveGame::StaticClass()));

		UE_LOG(LogTemp, Log, TEXT("Successfully created new SaveGame Object"));
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

void ASGameModeBase::GiveCredits(int32 Amount)
{
	TObjectPtr<ASCharacter> Player = Cast<ASCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));
	if (Player)
	{
		Player->GetPlayerState<ASPlayerState>()->AddCredits(Amount);
	}
}