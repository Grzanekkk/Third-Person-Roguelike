// Made by Jan Puto 2022 :D


#include "FunctionLibrary/GameplayFunctionLibrary.h"
#include "Components/SAttributeComponent.h"

bool UGameplayFunctionLibrary::ApplyDamage(AActor* DamageCauser, AActor* TargetActor, float DamageAmount)
{
	TObjectPtr<USAttributeComponent> AttribComponent = USAttributeComponent::GetAttributeComponent(TargetActor);
	if (AttribComponent)
	{
		return AttribComponent->ApplyHealthChange(DamageCauser, -DamageAmount);
	}

	return false;
}

bool UGameplayFunctionLibrary::ApplyDirectionalDamage(AActor* DamageCauser, AActor* TargetActor, float DamageAmount, const FHitResult& HitResult)
{
	if (ApplyDamage(DamageCauser, TargetActor, DamageAmount))
	{
		TObjectPtr<UPrimitiveComponent> HitComp = HitResult.GetComponent();
		if (HitComp && HitComp->IsSimulatingPhysics(HitResult.BoneName))
		{
			// Direction = Target - Origin !
			FVector Direction = HitResult.TraceEnd - HitResult.TraceStart;
			Direction.Normalize();

			HitComp->AddImpulseAtLocation(Direction * 500000.f, HitResult.ImpactPoint, HitResult.BoneName);
		}

		return true;
	}
	return false;
}
