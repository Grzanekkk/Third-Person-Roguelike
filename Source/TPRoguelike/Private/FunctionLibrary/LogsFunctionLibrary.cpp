// Made by Jan Puto 2022 :D


#include "FunctionLibrary/LogsFunctionLibrary.h"
#include "Logging/LogVerbosity.h"

void ULogsFunctionLibrary::LogOnScreen_IsClientServer(UObject* WorldContext, FString Msg, ERogueLogCategory LogCategory, float Duration, bool bWriteToOutputLog)
{
	if (!ensure(WorldContext))
	{
		return;
	}

	TObjectPtr<UWorld> World = WorldContext->GetWorld();
	if (!ensure(World))
	{
		return;
	}

	FString NetPrefix = World->IsNetMode(NM_Client) ? "[CLIENT] " : "[SERVER ]";
	if (GEngine)
	{
		FColor LogColor = GetLogColorByCategory(LogCategory);
		GEngine->AddOnScreenDebugMessage(-1, Duration, LogColor, NetPrefix + Msg);
	}
}

void ULogsFunctionLibrary::LogOnScreen(UObject* WorldContext, FString Msg, ERogueLogCategory LogCategory, bool bWriteToOutputLog, float Duration)
{
	if (GEngine)
	{
		FColor LogColor = GetLogColorByCategory(LogCategory);

		GEngine->AddOnScreenDebugMessage(-1, Duration, LogColor, Msg);

		if (bWriteToOutputLog)
		{
			LogToOutputLog(WorldContext, Msg, LogCategory);
		}
	}
}

void ULogsFunctionLibrary::LogToOutputLog(UObject* WorldContext, FString Msg, ERogueLogCategory LogCategory)
{
	switch (LogCategory)
	{
		case ERogueLogCategory::LOG:
		{
			UE_LOG(LogTemp, Log, TEXT("%s"), *Msg);
			break;
		}
		case ERogueLogCategory::WARNING:
		{
			UE_LOG(LogTemp, Warning, TEXT("%s"), *Msg);
			break;
		}
		case ERogueLogCategory::SUCCESS:
		{
			UE_LOG(LogTemp, Warning, TEXT("%s"), *Msg);
			break;
		}
		case ERogueLogCategory::ERROR:
		{
			UE_LOG(LogTemp, Error, TEXT("%s"), *Msg);
			break;
		}
	}
}

FColor ULogsFunctionLibrary::GetLogColorByCategory(ERogueLogCategory LogCategory)
{
	FColor LogColor = FColor::White;
	switch (LogCategory)
	{
		case ERogueLogCategory::LOG:
		{
			LogColor = FColor::White;
			break;
		}
		case ERogueLogCategory::WARNING:
		{
			LogColor = FColor::Orange;
			break;
		}
		case ERogueLogCategory::SUCCESS:
		{
			LogColor = FColor::Green;
			break;
		}
		case ERogueLogCategory::ERROR:
		{
			LogColor = FColor::Red;
			break;
		}
	}

	return LogColor;
}
