// Made by Jan Puto 2022 :D

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "GameplayTagContainer.h"
#include "SGameModeBase.generated.h"


class ASPickupBase;
class UCurveFloat;
class UEnvQuery;
class UEnvQueryInstanceBlueprintWrapper;
class USSaveGame;
class USObjectiveSequenceDataAsset;

UCLASS()
class TPROGUELIKE_API ASGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	

public:
	ASGameModeBase();

	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage);

	void HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer) override;

	virtual void StartPlay() override;

	virtual void OnActorKilled(AActor* VictimActor, AActor* Killer);

	////////////////////////////////////////////////////
	/// Cheats*
	UFUNCTION(Exec)
	void KillAllAI();

	UFUNCTION(Exec)
	void GiveCredits(int32 Amount);

protected:

	////////////////////////////////////////////////////
	/// Bots Spawning
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Rogue|Spawning|AI")
	bool bCanSpawnBots = true;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Rogue|Spawning|AI")
	TObjectPtr<UEnvQuery> SpawnBotQuery;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Rogue|Spawning|AI")
	TObjectPtr<UEnvQuery> SpawnPickupsQuery;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Rogue|Spawning|AI")
	TSubclassOf<AActor> MinionClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Rogue|Spawning|AI")
	TObjectPtr<UCurveFloat>  DifficultyCurve;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Rogue|Spawning|AI")
	float SpawnTimerInterval = 2.f;

	// It will use this value if we dont have DifficultyCurve assigned (we should assigne it thoe)
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Rogue|Spawning|AI")
	int32 MaxBotCount = 10;

	UPROPERTY()
	FTimerHandle TimerHandle_SpawnBot;

	UFUNCTION()
	void SpawnBotTimerElapsed();

	UFUNCTION()
	void OnSpawnBotQueryCompleted(UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus);

	////////////////////////////////////////////////////
	/// Player Respawning
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Rogue|Player Respawn")
	float PlayerRespawnDelay = 3.f;

	UFUNCTION()
	void RespawnPlayerElapsed(AController* PlayerController);

	////////////////////////////////////////////////////
	/// Pickup Spawning
	UFUNCTION()
	void SpawnPickups();

	UFUNCTION()
	void OnSpawnQueryCompleted(UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus);

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Rogue|Spawning|Pickups", meta = (ClampMin = 0, ClampMax = 100))
	float PickupSpawnChancePercentage = 100.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Rogue|Spawning|Pickups")
	TArray<TSubclassOf<ASPickupBase>> PickUpsAvalibleToSpawn;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Rogue|Spawning|Pickups")
	bool bCanSpawnPickups = true;

	////////////////////////////////////////////////////
	/// SaveSystem
	UFUNCTION(BlueprintCallable, Category = "Rogue|SaveSystem")
	void WriteSaveGame();

	UFUNCTION(BlueprintCallable, Category = "Rogue|SaveSystem")
	void LoadSaveGame();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Rogue|SaveSystem")
	FString SaveSlotName = "Save001";

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Rogue|SaveSystem")
	TObjectPtr<USSaveGame> CurrentSaveGame;

	////////////////////////////////////////////////////
	/// Quests

	UFUNCTION()
	void StartStartingObjectives();

	UFUNCTION()
	void StartStartingObjectiveSequance();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Rogue|Quests")
	FGameplayTagContainer StartingObjectives;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Rogue|Quests")
	TObjectPtr<USObjectiveSequenceDataAsset> StartingObjectiveSequance;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Rogue|Quests")
	bool bStartObjectiveSequanceInsteadOfSingleObjective = true;
};