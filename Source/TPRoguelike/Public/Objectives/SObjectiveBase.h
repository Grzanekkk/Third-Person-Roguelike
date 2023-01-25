// Made by Jan Puto 2023 :D

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "SObjectiveBase.generated.h"

class USQuestBase;
enum class EObjectiveState : uint8;
enum class EObjectiveType : uint8;

/**
 *
 */
UCLASS(Blueprintable)
class TPROGUELIKE_API USObjectiveBase : public UObject
{
	GENERATED_BODY()

public:
	USObjectiveBase();

	UFUNCTION()
	virtual void ServerOnlyStartObjective();

	UFUNCTION(Client, Reliable)
	virtual void ClientStartObjective();

	UFUNCTION()
	virtual void ServerOnlyFinishObjective();

	UFUNCTION(Client, Reliable)
	virtual void ClientFinishObjective();

	UFUNCTION()
	virtual bool CanStartObjective();

	UFUNCTION()
	virtual void ServerOnlyOnObjectiveFinished();

	UFUNCTION()
	void Initialize(USQuestBase* OuterQuestPtr);

protected:
	UFUNCTION()
	void OnRep_ObjectiveState();

	UPROPERTY(ReplicatedUsing="OnRep_ObjectiveState")
	EObjectiveState ObjectiveState;

	UPROPERTY()
	EObjectiveType ObjectiveType;

	UPROPERTY()
	TObjectPtr<USQuestBase> OuterQuest = nullptr;

	UPROPERTY()
	FText ObjectiveName;
};

