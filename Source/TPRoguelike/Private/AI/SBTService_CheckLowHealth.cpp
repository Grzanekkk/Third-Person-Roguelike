// Made by Jan Puto 2022 :D


#include "AI/SBTService_CheckLowHealth.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "AI/SAICharacter.h"
#include "Components/SAttributeComponent.h"

void USBTService_CheckLowHealth::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	// Check if Ai Actor is low health
	TObjectPtr<UBlackboardComponent> BlackBoardComp = OwnerComp.GetBlackboardComponent();
	if (ensure(BlackBoardComp))
	{
		TObjectPtr<AAIController> AIController = OwnerComp.GetAIOwner();
		if (ensure(AIController))
		{
			TObjectPtr<ASAICharacter> AICharacter = Cast<ASAICharacter>(AIController->GetPawn());
			if (ensure(AICharacter))
			{
				TObjectPtr<USAttributeComponent> AttribComponent = USAttributeComponent::GetAttributeComponent(AICharacter);
				if(ensure(AttribComponent))
				{
					bool bIsLowHealth = !AttribComponent->IsHealthHigherThen(LowHealthThreshold);

					BlackBoardComp->SetValueAsBool(IsLowHelthKey.SelectedKeyName, bIsLowHealth);
				}
			}
		}
	}
}