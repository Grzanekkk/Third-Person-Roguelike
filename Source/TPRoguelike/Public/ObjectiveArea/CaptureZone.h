// Made by Jan Puto 2022 :D

#pragma once

#include "CoreMinimal.h"
#include "ObjectiveArea/ObjectiveArea.h"
#include "CaptureZone.generated.h"

class UCapsuleComponent;

/**
 * 
 */
UCLASS()
class TPROGUELIKE_API ACaptureZone : public AObjectiveArea
{
	GENERATED_BODY()
	
public:
	ACaptureZone();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Rogue|Components")
	UCapsuleComponent* TriggerBox = nullptr;

protected:
	virtual void BeginPlay() override;
};
