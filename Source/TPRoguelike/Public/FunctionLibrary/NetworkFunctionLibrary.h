// Made by Jan Puto 2023 :D

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "NetworkFunctionLibrary.generated.h"

/**
 * 
 */
UCLASS()
class TPROGUELIKE_API UNetworkFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Rogue|Getters")
	static APlayerController* GetLocalPlayerController(const UObject* WorldContextObject);
};
