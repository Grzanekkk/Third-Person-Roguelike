// Made by Jan Puto 2022 :D

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "SSaveGame.generated.h"

USTRUCT()
struct FActorSavedData
{
	GENERATED_BODY()

public:

	UPROPERTY()
	FString ActorName;

	UPROPERTY()
	FTransform ActorTransform;

	UPROPERTY()
	TArray<uint8> ByteData;
};

/**
 * 
 */
UCLASS()
class TPROGUELIKE_API USSaveGame : public USaveGame
{
	GENERATED_BODY()
	
public: 
	UPROPERTY()
	int32 Credits;

	UPROPERTY()
	TArray<FActorSavedData> SavedActors;
};
