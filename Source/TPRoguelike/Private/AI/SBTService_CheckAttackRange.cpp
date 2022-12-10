// Made by Jan Puto 2022 :D


#include "AI/SBTService_CheckAttackRange.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"

void USBTService_CheckAttackRange::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	// Check distance between ai pawn and target actor

	TObjectPtr<UBlackboardComponent> BlackBoardComp = OwnerComp.GetBlackboardComponent();
	if (ensure(BlackBoardComp))
	{
		TObjectPtr<AActor> TargetActor = Cast<AActor>(BlackBoardComp->GetValueAsObject("TargetActor"));
		if (TargetActor)	// Can often be null
		{
			TObjectPtr<AAIController> AIController = OwnerComp.GetAIOwner();
			if (ensure(AIController))
			{
				TObjectPtr<APawn> AIPawn = AIController->GetPawn();
				if (ensure(AIPawn))
				{
					float DistanceTo = FVector::Distance(AIPawn->GetActorLocation(), TargetActor->GetActorLocation());

					//UE_LOG(LogTemp, Warning, TEXT("Distance from %s to %s: %f"), *GetNameSafe(AIPawn), *GetNameSafe(TargetActor), DistanceTo);

					bool bWithinRange = DistanceTo < AcceptableDistance;

					bool bHasLineOfSight = false;
					if (bWithinRange)
					{
						bHasLineOfSight = AIController->LineOfSightTo(TargetActor);
					}

					BlackBoardComp->SetValueAsBool(AttackRangeKey.SelectedKeyName, bWithinRange && bHasLineOfSight);
				}
			}
		}
	}
}