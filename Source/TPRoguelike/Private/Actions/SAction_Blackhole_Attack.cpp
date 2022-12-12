// Made by Jan Puto 2022 :D


#include "Actions/SAction_Blackhole_Attack.h"
#include "Components/SActionComponent.h"
#include "Components/SAttributeComponent.h"

bool USAction_Blackhole_Attack::CanStart_Implementation(AActor* Instigator)
{
	bool bCanStart = true;//Super::CanStart(Instigator);
	if (bCanStart)
	{
		TObjectPtr<USAttributeComponent> AttributeComponent = Cast< USAttributeComponent>(GetOuterComponent()->GetOwner()->GetComponentByClass(USAttributeComponent::StaticClass()));
		if (AttributeComponent)
		{
			bCanStart = AttributeComponent->ApplyRageChange(Instigator, -RageCost);
		}
	}

	return bCanStart;
}
