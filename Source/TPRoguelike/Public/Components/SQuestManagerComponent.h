// Made by Jan Puto 2022 :D

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
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

	UFUNCTION(Server, Reliable)
	void ServerStartQuestByClass(const TSoftClassPtr<USQuestBase>& QuestClass);

	UFUNCTION()
	void ServerOnlyFinishQuestByClass(const TSoftClassPtr<USQuestBase>& QuestClass, bool bQuestFinishedSuccessfully);

	UFUNCTION(Server, Reliable)
	void ServerStartObjectiveByClass(const TSoftClassPtr<USObjectiveBase>& ObjectiveSoftClass, USQuestBase* InQuest);

	UFUNCTION()
	USQuestBase* FindActiveQuestByClass(const TSubclassOf<USQuestBase>& QuestClass);


protected:
	virtual void BeginPlay() override;

	UFUNCTION(NetMulticast, Reliable)
	void MulticastOnQuestStateChanged(USQuestBase* QuestInstance, EQuestState QuestState);

	UFUNCTION(NetMulticast, Reliable)
	void MulticastOnObjectiveStateChanged(USObjectiveBase* ObjectiveInstance, USQuestBase* QuestInstance, EObjectiveState ObjectiveState);

	UPROPERTY(BlueprintAssignable)
	FOnQuestStateChanged OnQuestStateChanged;

	UPROPERTY(BlueprintAssignable)
	FOnObjectiveStateChanged OnObjectiveStateChanged;

	UPROPERTY(Replicated)
	TArray<TObjectPtr<USQuestBase>> CurrentActiveQuests;

};
