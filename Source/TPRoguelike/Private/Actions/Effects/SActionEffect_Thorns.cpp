// Made by Jan Puto 2022 :D


#include "Actions/Effects/SActionEffect_Thorns.h"
#include "Components/SActionComponent.h"
#include "Components/SAttributeComponent.h"

void USActionEffect_Thorns::StartAction_Implementation(AActor* Instigator)
{
	// This has no checks for outerCompnentnt and Owner because for them to be null something would have to go terribly wrong
	TObjectPtr<USAttributeComponent> AttributeComponent = Cast<USAttributeComponent>(GetOuterComponent()->GetOwner()->GetComponentByClass(USAttributeComponent::StaticClass()));
	if (AttributeComponent)
	{
		AttributeComponent->OnHealthChanged.AddDynamic(this, &USActionEffect_Thorns::OnOwnerHealthChanged);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Trying to run SActionEffect_Thorns on Actor with on USAttributeComponent. This Action will not work here!"));
	}
}

void USActionEffect_Thorns::StopAction_Implementation(AActor* Instigator)
{
	TObjectPtr<USAttributeComponent> AttributeComponent = Cast<USAttributeComponent>(GetOuterComponent()->GetOwner()->GetComponentByClass(USAttributeComponent::StaticClass()));
	if (AttributeComponent)
	{
		AttributeComponent->OnHealthChanged.RemoveDynamic(this, &USActionEffect_Thorns::OnOwnerHealthChanged);
	}
}

void USActionEffect_Thorns::OnOwnerHealthChanged(AActor* InstigatorActor, USAttributeComponent* OwningComp, float NewHealth, float DeltaHealth)
{
	TObjectPtr<AActor> Owner = GetOuterComponent()->GetOwner();
	if (InstigatorActor != Owner)
	{
		int32 ThornsDamage = FMath::RoundToInt32(DeltaHealth * ThornsMultiplier);

		TObjectPtr<USAttributeComponent> InstigatorAttributeComponent = Cast<USAttributeComponent>(InstigatorActor->GetComponentByClass(USAttributeComponent::StaticClass()));
		{
			InstigatorAttributeComponent->ApplyHealthChange(Owner, ThornsDamage);
			UE_LOG(LogTemp, Warning, TEXT("%s delt %i Thorns Damage to %s"), *GetNameSafe(Owner), -ThornsDamage, *GetNameSafe(InstigatorActor));
		}
	}
}
