// Made by Jan Puto 2023 :D


#include "FunctionLibrary/NetworkFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"

APlayerController* UNetworkFunctionLibrary::GetLocalPlayerController(const UObject* WorldContextObject)
{
	for (int i = 0; i < UGameplayStatics::GetNumPlayerControllers(WorldContextObject); i++)
	{
		TObjectPtr<APlayerController> Controller = UGameplayStatics::GetPlayerController(WorldContextObject, i);
		if (Controller->IsLocalController())
		{
			return Controller;
		}
	}

	return nullptr;
}
