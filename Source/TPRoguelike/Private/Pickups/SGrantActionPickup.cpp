// Made by Jan Puto 2022 :D


#include "Pickups/SGrantActionPickup.h"
#include "Components/SActionComponent.h"
#include "Actions/SAction.h"

void ASGrantActionPickup::UsePickupItem(APawn* InstigatorPawn)
{
	Super::UsePickupItem(InstigatorPawn);

	TObjectPtr<USActionComponent> ActionComponent = Cast<USActionComponent>(InstigatorPawn->GetComponentByClass(USActionComponent::StaticClass()));
	if (ActionComponent)
	{
		ActionComponent->AddAction(InstigatorPawn, ActionToGrant);

		Destroy();
	}
}