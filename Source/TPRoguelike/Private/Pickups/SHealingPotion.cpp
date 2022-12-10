// Made by Jan Puto 2022 :D


#include "Pickups/SHealingPotion.h"
#include "Components/SAttributeComponent.h"

ASHealingPotion::ASHealingPotion()
{

}

void ASHealingPotion::BeginPlay()
{
	Super::BeginPlay();
}

void ASHealingPotion::Interact_Implementation(APawn* InstigatorPawn)
{
	UsePickupItem(InstigatorPawn);
}

bool ASHealingPotion::CanInteract_Implementation(APawn* InstigatorPawn)
{
	if (Super::CanInteract_Implementation(InstigatorPawn))
	{
		USAttributeComponent* AttributeComponent = USAttributeComponent::GetAttributeComponent(InstigatorPawn);
		if (AttributeComponent)
		{
			if (!AttributeComponent->IsFullyHealed())
			{
				return true;
			}
		}
	}

	return false;
}

void ASHealingPotion::UsePickupItem(APawn* InstigatorPawn)
{
	Super::UsePickupItem(InstigatorPawn);

	USAttributeComponent* AttributeComponent = USAttributeComponent::GetAttributeComponent(InstigatorPawn);
	if (AttributeComponent)
	{
		AttributeComponent->ApplyHealthChange(this, HealingAmount);

		ASPickupBase::UsePickupItem(InstigatorPawn);
	}
}