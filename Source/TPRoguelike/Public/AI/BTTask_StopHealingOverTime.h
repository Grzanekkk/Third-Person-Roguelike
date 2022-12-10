// Made by Jan Puto 2022 :D

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_StopHealingOverTime.generated.h"

/**
 * 
 */
UCLASS()
class TPROGUELIKE_API UBTTask_StopHealingOverTime : public UBTTaskNode
{
	GENERATED_BODY()
	
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
