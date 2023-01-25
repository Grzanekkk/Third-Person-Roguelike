// Made by Jan Puto 2023 :D

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "SQuestBase.generated.h"

class USQuestManagerComponent;
class USObjectiveBase;
enum class EQuestState : uint8;

/**
 * 
 */
UCLASS(Blueprintable)
class TPROGUELIKE_API USQuestBase : public UObject
{
	GENERATED_BODY()
	
public:
	USQuestBase();

	/* Its not marked as "Server" because we are only going to call this function from the server on the server */
	UFUNCTION()
	virtual void ServerOnlyStartQuest();

	UFUNCTION(Client, Reliable)
	virtual void ClientStartQuest();

	UFUNCTION()
	virtual void ServerOnlyFinishQuest();

	UFUNCTION(Client, Reliable)
	virtual void ClientFinishQuest();

	UFUNCTION()
	virtual bool CanStartQuest();

	UFUNCTION()
	virtual bool ServerOnlyStartObjectiveByClass(const TSoftClassPtr<USObjectiveBase>& ObjectiveSoftClass);

	// Only call this from the manager
	UFUNCTION()
	virtual void ServerOnlyOnAllObjectivesFinished();

	UFUNCTION()
	void Initialize(USQuestManagerComponent* InOuterComponent);


	/// GETTERS
	UFUNCTION()
	USObjectiveBase* GetActiveObjectiveByClass(const TSoftClassPtr<USObjectiveBase>& ObjectiveSoftClass);

protected:

	UPROPERTY()
	TObjectPtr<USQuestManagerComponent> OuterComponent = nullptr;

	UPROPERTY(ReplicatedUsing="OnRep_QuestState")
	EQuestState QuestState;

	UFUNCTION()
	void OnRep_QuestState();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Rogue|Quest")
	FText QuestName;

	UPROPERTY(Replicated)
	TArray<TObjectPtr<USObjectiveBase>> ActiveObjectives;

	UPROPERTY()
	TArray<TSoftClassPtr<USObjectiveBase>> StartingObjectives;
};