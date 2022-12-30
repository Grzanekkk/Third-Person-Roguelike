// Made by Jan Puto 2022 :D


#include "Controllers/SPlayerController.h"

void ASPlayerController::SetPawn(APawn* InPawn)
{
	Super::SetPawn(InPawn);

	OnPawnChanged.Broadcast(InPawn);
}
