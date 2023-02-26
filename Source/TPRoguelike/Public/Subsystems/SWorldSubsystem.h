// Made by Jan Puto 2023 :D

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "SWorldSubsystem.generated.h"

/**
 * 
 */
UCLASS()
class TPROGUELIKE_API USWorldSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()
	
public:
	UFUNCTION()
	void SetObjectiveDataForLevelAndStart();
};
