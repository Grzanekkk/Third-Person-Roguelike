// Made by Jan Puto 2022 :D

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "SGameModeBase.generated.h"


class UEnvQuery;
class UEnvQueryInstanceBlueprintWrapper;
class UCurveFloat;
class ASPickupBase;

UCLASS()
class TPROGUELIKE_API ASGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
protected:

	////////////////////////////////////////////////////
	/// Bots Spawning
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Spawning|AI")
	bool bCanSpawnBots = true;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Spawning|AI")
	TObjectPtr<UEnvQuery> SpawnBotQuery;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Spawning|AI")
	TObjectPtr<UEnvQuery> SpawnPickupsQuery;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Spawning|AI")
	TSubclassOf<AActor> MinionClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Spawning|AI")
	TObjectPtr<UCurveFloat>  DifficultyCurve;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Spawning|AI")
	float SpawnTimerInterval = 2.f;

	// It will use this value if we dont have DifficultyCurve assigned (we should assigne it thoe)
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Spawning|AI")
	int32 MaxBotCount = 10;

	UPROPERTY()
	FTimerHandle TimerHandle_SpawnBot;

	UFUNCTION()
	void SpawnBotTimerElapsed();

	UFUNCTION()
	void OnSpawnBotQueryCompleted(UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus);


	////////////////////////////////////////////////////
	/// Player Respawning
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player Respawn")
	float PlayerRespawnDelay = 3.f;

	UFUNCTION()
	void RespawnPlayerElapsed(AController* PlayerController);


	////////////////////////////////////////////////////
	/// Pickup Spawning
	UFUNCTION()
	void SpawnPickups();

	UFUNCTION()
	void OnSpawnQueryCompleted(UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus);

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Spawning|Pickups", meta = (ClampMin = 0, ClampMax = 100))
	float PickupSpawnChancePercentage = 100.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Spawning|Pickups")
	TArray<TSubclassOf<ASPickupBase>> PickUpsAvalibleToSpawn;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Spawning|Pickups")
	bool bCanSpawnPickups = true;

public:
	ASGameModeBase();

	virtual void StartPlay() override;

	////////////////////////////////////////////////////
	/// OnActorKilled
	virtual void OnActorKilled(AActor* VictimActor, AActor* Killer);


	////////////////////////////////////////////////////
	/// Cheats
	UFUNCTION(Exec)
	void KillAllAI();
};
