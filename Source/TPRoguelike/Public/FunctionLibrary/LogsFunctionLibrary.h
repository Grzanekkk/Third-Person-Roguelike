// Made by Jan Puto 2022 :D

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Enums/SEnums_Logs.h"
#include "LogsFunctionLibrary.generated.h"

//enum class ERogueLogCategory : uint8;

/**
 * 
 */
UCLASS()
class TPROGUELIKE_API ULogsFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, Category = "Rogue|Logs")
	static void LogOnScreen_IsClientServer(UObject* WorldContext, FString Msg, ERogueLogCategory LogCategory, float Duration = 5.0f, bool bWriteToOutputLog = true);

	UFUNCTION(BlueprintCallable, Category = "Rogue|Logs")
	static void LogOnScreen(UObject* WorldContext, FString Msg, ERogueLogCategory LogCategory, bool bWriteToOutputLog = true, float Duration = 3.0f);
	
	UFUNCTION(BlueprintCallable, Category = "Rogue|Logs")
	static void LogToOutputLog(UObject* WorldContext, FString Msg, ERogueLogCategory LogCategory);

protected:
	UFUNCTION()
	static FColor GetLogColorByCategory(ERogueLogCategory LogCategory);
};