// Made by Jan Puto 2022 :D

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_StartHealingOverTime.generated.h"

/**
 * 
 */
UCLASS()
class TPROGUELIKE_API UBTTask_StartHealingOverTime : public UBTTaskNode
{
	GENERATED_BODY()
	
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

public:
	UPROPERTY(EditAnywhere, Category = "AI")
	float HealthPerSecond = 4;

	UPROPERTY(EditAnywhere, Category = "AI")
	float SecondsOfHealing = 3;
};
