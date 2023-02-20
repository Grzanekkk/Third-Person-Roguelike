// Made by Jan Puto 2022 :D


#include "FunctionLibrary/LogsFunctionLibrary.h"
#include "Enums/SEnums_Logs.h"
#include "Logging/LogVerbosity.h"

void ULogsFunctionLibrary::LogOnScreen_IsClientServer(UObject* WorldContext, FString Msg, FColor Color, float Duration)
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
		GEngine->AddOnScreenDebugMessage(-1, Duration, Color, NetPrefix + Msg);
	}
}

void ULogsFunctionLibrary::LogOnScreen(UObject* WorldContext, FString Msg, ERogueLogCategory LogCategory, bool bWriteToOutputLog, float Duration)
{
	if (GEngine)
	{
		FColor LogColor = FColor::White;
		//ELogVerbosity::Type LogVerbosityType = ELogVerbosity::Type::Log;
		switch (LogCategory)
		{
			case ERogueLogCategory::LOG :
			{
				LogColor = FColor::White;
				//LogVerbosityType = ELogVerbosity::Type::Log;
			}
			case ERogueLogCategory::WARNING :
			{
				LogColor = FColor::Orange;
				//LogVerbosityType = ELogVerbosity::Type::Warning;
			}
			case ERogueLogCategory::SUCCESS:
			{
				LogColor = FColor::Green;
				//LogVerbosityType = ELogVerbosity::Type::Display;
			}
			case ERogueLogCategory::ERROR:
			{
				LogColor = FColor::Red;
				//LogVerbosityType = ELogVerbosity::Type::Error;
			}
		}

		GEngine->AddOnScreenDebugMessage(-1, Duration, LogColor, Msg);

		if (bWriteToOutputLog)
		{
			//UE_LOG(LogTemp, LogVerbosityType, TEXT("%s"), *Msg);

			switch (LogCategory)
			{
				case ERogueLogCategory::LOG:
				{
					UE_LOG(LogTemp, Log, TEXT("%s"), *Msg);
				}
				case ERogueLogCategory::WARNING:
				{
					UE_LOG(LogTemp, Warning, TEXT("%s"), *Msg);
				}
				case ERogueLogCategory::SUCCESS:
				{
					UE_LOG(LogTemp, Display, TEXT("%s"), *Msg);
				}
				case ERogueLogCategory::ERROR:
				{
					UE_LOG(LogTemp, Error, TEXT("%s"), *Msg);
				}
			}
		}
	}
}
