// Made by Jan Puto 2022 :D

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "SGameState.generated.h"

class USQuestManagerComponent;
class ACaptureZone;

/**
*
**/
USTRUCT()
struct FQuestCaptureZone
{
	GENERATED_BODY()

public:
	UPROPERTY()
	TObjectPtr<ACaptureZone> CaptureZone = nullptr;

	UPROPERTY()
	bool bHasQuestAssigned = false;
};


/**
 * 
 */
UCLASS()
class TPROGUELIKE_API ASGameState : public AGameStateBase
{
	GENERATED_BODY()

public:
	ASGameState();

	UFUNCTION(BlueprintPure, BlueprintCallable, Category = "Rogue|Getters")
	FORCEINLINE USQuestManagerComponent* GetQuestManager() const { return QuestManagerComponent; }

	UFUNCTION(BlueprintPure, BlueprintCallable, Category = "Rogue|Getters")
	int32 GetNumberOfConnectedPlayers() const;
	
protected:
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<USQuestManagerComponent> QuestManagerComponent = nullptr;


};