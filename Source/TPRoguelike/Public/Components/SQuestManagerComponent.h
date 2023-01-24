// Made by Jan Puto 2022 :D

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SQuestManagerComponent.generated.h"

class USQuestBase;
enum class EQuestState : uint8;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnQuestStateChanged, TSubclassOf<USQuestBase>, QuestClass, EQuestState, QuestState);


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

	UPROPERTY(BlueprintAssignable)
	FOnQuestStateChanged OnQuestStateChanged;

protected:
	virtual void BeginPlay() override;

	UFUNCTION(NetMulticast, Reliable)
	void MulticastOnQuestStateChanged(TSubclassOf<USQuestBase> QuestClass, EQuestState QuestState);

	UPROPERTY(Replicated)
	TObjectPtr<USQuestBase> CurrentActiveQuest = nullptr;

};
