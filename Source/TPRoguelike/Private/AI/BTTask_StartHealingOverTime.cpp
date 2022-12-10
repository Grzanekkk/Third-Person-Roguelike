// Made by Jan Puto 2022 :D


#include "AI/BTTask_StartHealingOverTime.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AI/SAICharacter.h"

EBTNodeResult::Type UBTTask_StartHealingOverTime::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	TObjectPtr<AAIController> OwnerController = OwnerComp.GetAIOwner();
	if (ensure(OwnerController))
	{
		TObjectPtr<ASAICharacter> AICharacter = Cast<ASAICharacter>(OwnerController->GetPawn());
		if (ensure(AICharacter))
		{
			AICharacter->StartHealingOverTime(HealthPerSecond, SecondsOfHealing);

			return EBTNodeResult::Succeeded;
		}
	}

	return EBTNodeResult::Failed;
}