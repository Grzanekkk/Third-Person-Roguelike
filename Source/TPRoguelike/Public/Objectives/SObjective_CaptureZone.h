// Made by Jan Puto 2023 :D

#pragma once

#include "CoreMinimal.h"
#include "Objectives/SObjectiveBase.h"
#include "SObjective_CaptureZone.generated.h"

class ACaptureZone;
class ASCharacter;

/**
 * 
 */
UCLASS()
class TPROGUELIKE_API USObjective_CaptureZone : public USObjectiveBase
{
	GENERATED_BODY()

//public:
//
//	USObjective_CaptureZone();
//
//	virtual void ServerOnlyStartObjective() override;
//
//	virtual void ServerOnlyFinishObjective() override;
//
//	virtual bool CanStartObjective() override;
//
//	// virtual void ServerOnlyOnAllObjectivesFinished() override;
//
//protected:
//	UFUNCTION()
//	void OnZoneCaptured(ACaptureZone* CapturedZone, TArray<ASCharacter*> PlayersResponsibleForCapture);
};
