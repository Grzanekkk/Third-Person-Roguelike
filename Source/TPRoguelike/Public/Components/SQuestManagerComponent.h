// Made by Jan Puto 2022 :D

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameplayTagContainer.h"
#include "SQuestManagerComponent.generated.h"

class USQuestBase;
class USObjectiveBase;
class USQuestDataAsset;
enum class EQuestState : uint8;
enum class EObjectiveState : uint8;

struct FObjectiveReplicationData
{
	GENERATED_BODY();

public:
	UPROPERTY()
	FGameplayTag Tag;

	UPROPERTY()
	int32 Value;

	UPROPERTY()
	EObjectiveState ObjectiveState;

	FObjectiveReplicationData(FGameplayTag _Tag, int32 _Value, EObjectiveState _ObjectiveState)
	{
		Tag = _Tag;
		Value = _Value;
		ObjectiveState = _ObjectiveState;
	}
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnObjectiveStateChanged, FGameplayTag, ObjectiveTag, EObjectiveState, ObjectiveState);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnObjectiveValueChanged, FGameplayTag, ObjectiveTag, int32, CurrentValue, int32, DeltaValue);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TPROGUELIKE_API USQuestManagerComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	USQuestManagerComponent();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void ServerOnlyAddObjectiveStat(FGameplayTag ObjectiveTag, int32 Stat);

	void ServerOnlyStartObjective(FGameplayTag ObjectiveTag);

protected:
	virtual void BeginPlay() override;

	UPROPERTY(BlueprintAssignable)
	FOnObjectiveStateChanged OnObjectiveStateChangedEvent;

	UPROPERTY(BlueprintAssignable)
	FOnObjectiveValueChanged OnObjectiveValueChanged;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Rogue|Quests")
	TMap<FGameplayTag, int32> ActiveObjectivesState;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Rogue|Quests")
	TObjectPtr<USQuestDataAsset> DefalutObjectivesGoals;

	TArray<FObjectiveReplicationData> LocalObjectiveData;

	UPROPERTY(ReplicatedUsing="OnRep_ServerObjectiveData")
	TArray<FObjectiveReplicationData> ServerObjectiveData;

	UFUNCTION()
	void OnRep_ServerObjectiveData();

};
