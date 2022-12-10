// Made by Jan Puto 2022 :D

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "SBTService_CheckLowHealth.generated.h"


UCLASS()
class TPROGUELIKE_API USBTService_CheckLowHealth : public UBTService
{
	GENERATED_BODY()
	
protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	UPROPERTY(EditAnywhere, Category = "AI")
	float LowHealthThreshold = 30.f;

	UPROPERTY(EditAnywhere, Category = "AI")
	FBlackboardKeySelector IsLowHelthKey;
};
