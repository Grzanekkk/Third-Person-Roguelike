// Made by Jan Puto 2022 :D

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameplayTagContainer.h"
#include "SQuestManagerComponent.generated.h"

class USQuestBase;
class USObjectiveBase;
enum class EQuestState : uint8;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnQuestStateChanged, USQuestBase*, QuestInstance, EQuestState, QuestState);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnObjectiveStateChanged, USObjectiveBase*, ObjectiveInstance, USQuestBase*, QuestInstance, EObjectiveState, ObjectiveState);


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TPROGUELIKE_API USQuestManagerComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	USQuestManagerComponent();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void AddObjectiveStat(FGameplayTag ObjectiveTag, int32 Stat);

protected:
	virtual void BeginPlay() override;

	UPROPERTY(BlueprintAssignable)
	FOnQuestStateChanged OnQuestStateChangedEvent;

	UPROPERTY(BlueprintAssignable)
	FOnObjectiveStateChanged OnObjectiveStateChangedEvent;

	UPROPERTY(EditDefaultsOnly, Category = "Rogue|Quests")
	TObjectPtr<UDataAsset> ActiveObjectives;

};
