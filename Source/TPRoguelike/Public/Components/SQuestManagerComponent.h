// Made by Jan Puto 2022 :D

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameplayTagContainer.h"
#include "SQuestManagerComponent.generated.h"

class USQuestBase;
class USObjectiveBase;
class USQuestDataAsset;
class USObjectiveSequenceDataAsset;
enum class EQuestState : uint8;
enum class EObjectiveState : uint8;

USTRUCT(Blueprintable)
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

	FObjectiveReplicationData() {};

	FObjectiveReplicationData(FGameplayTag _Tag, int32 _Value, EObjectiveState _ObjectiveState)
	{
		Tag = _Tag;
		Value = _Value;
		ObjectiveState = _ObjectiveState;
	}
};

USTRUCT(Blueprintable)
struct FObjectiveSquanceStep
{
	GENERATED_BODY();

public:
	UPROPERTY()
	FGameplayTag ObjectiveTag;

	UPROPERTY()
	EObjectiveState ObjectiveState;

	FObjectiveSquanceStep() {};

	FObjectiveSquanceStep(FGameplayTag _ObjectiveTag, EObjectiveState _ObjectiveState)
	{
		ObjectiveTag = _ObjectiveTag;
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

	void ServerOnlyAddObjectiveStat(FGameplayTag ObjectiveTag, int32 Stat);

	void ServerOnlyStartObjective(FGameplayTag ObjectiveTag);

	void ServerOnlyFinishObjectiveByRef(FObjectiveReplicationData& ObjectiveData);

	void ServerOnlyStartObjectiveSequance(TObjectPtr<USObjectiveSequenceDataAsset> ObjectiveSequance);

	UFUNCTION()
	bool IsObjectiveActive(FGameplayTag ObjectiveTag);

	UFUNCTION()
	bool IsObjectiveFinished(FGameplayTag ObjectiveTag);

protected:
	UFUNCTION()
	void ServerOnlyStartNextObjectiveInSequanceIfPossible(FGameplayTag FinishedObjectiveTag);

	UFUNCTION()
	void ChangeObjectiveStateByRef(FObjectiveReplicationData& ObjectiveData, EObjectiveState NewState);

	UFUNCTION()
	void ChangeObjectiveStateByTag(FGameplayTag ObjectiveTag, EObjectiveState NewState);

	UFUNCTION()
	void ChangeObjectiveValueByRef(FObjectiveReplicationData& ObjectiveData, int32 NewValue);
	
	UFUNCTION()
	void ChangeObjectiveValueByTag(FGameplayTag ObjectiveTag, int32 NewValue);

	UFUNCTION()
	int32 GetValueOfActiveObjective(FGameplayTag ObjectiveTag);

	UPROPERTY(BlueprintAssignable)
	FOnObjectiveStateChanged OnObjectiveStateChangedEvent;

	UPROPERTY(BlueprintAssignable)
	FOnObjectiveValueChanged OnObjectiveValueChanged;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Rogue|Quests")
	TArray<FGameplayTag> ActiveObjectivesState;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Rogue|Quests")
	TObjectPtr<USQuestDataAsset> DefalutObjectivesGoals;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Rogue|Quests")
	TObjectPtr<USObjectiveSequenceDataAsset> ActiveObjectiveSequance;

	//UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Rogue|Quests")
	//TArray<FObjectiveSquanceStep> CurrentObjectiveSequanceStep;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Rogue|Quests")
	TArray<FGameplayTag> CurrentObjectiveSequanceStep;

	// Z tego pobieramy informacje
	UPROPERTY()
	TArray<FObjectiveReplicationData> LocalObjectiveData;

	// Tutaj ustawiamy informacje
	UPROPERTY(ReplicatedUsing="OnRep_ServerObjectiveData")
	TArray<FObjectiveReplicationData> ServerObjectiveData;

	UFUNCTION()
	void OnRep_ServerObjectiveData();

};
