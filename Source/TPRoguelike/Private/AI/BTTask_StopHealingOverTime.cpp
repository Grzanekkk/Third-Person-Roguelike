// Made by Jan Puto 2022 :D


#include "AI/BTTask_StopHealingOverTime.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AI/SAICharacter.h"

EBTNodeResult::Type UBTTask_StopHealingOverTime::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	TObjectPtr<AAIController> OwnerController = OwnerComp.GetAIOwner();
	if (ensure(OwnerController))
	{
		TObjectPtr<ASAICharacter> AICharacter = Cast<ASAICharacter>(OwnerController->GetPawn());
		if (ensure(AICharacter))
		{
			AICharacter->StopHealingOverTime();

			return EBTNodeResult::Succeeded;
		}
	}

	return EBTNodeResult::Failed;
}