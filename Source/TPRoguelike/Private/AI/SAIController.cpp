// Made by Jan Puto 2022 :D


#include "AI/SAIController.h"
#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BlackboardComponent.h"

void ASAIController::BeginPlay()
{
	Super::BeginPlay();

	if (ensureMsgf(BehaviorTree, TEXT("BehaviorTree is missing! Please assigne BehaviorTree in AIController")))
	{
		RunBehaviorTree(BehaviorTree);
	}
}