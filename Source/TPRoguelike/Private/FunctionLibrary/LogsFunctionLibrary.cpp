// Made by Jan Puto 2022 :D


#include "FunctionLibrary/LogsFunctionLibrary.h"

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
