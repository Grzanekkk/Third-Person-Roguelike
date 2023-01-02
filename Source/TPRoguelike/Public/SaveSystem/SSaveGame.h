// Made by Jan Puto 2022 :D

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "SSaveGame.generated.h"

/**
 * 
 */
UCLASS()
class TPROGUELIKE_API USSaveGame : public USaveGame
{
	GENERATED_BODY()
	
public: 
	UPROPERTY()
	int32 Credits = 0;

};
