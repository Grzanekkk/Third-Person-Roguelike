// Made by Jan Puto 2022 :D


#include "Components/SAttributeComponent.h"
#include "GameModes/SGameModeBase.h"


static TAutoConsoleVariable<float> CVarDamageMultipler(TEXT("jp.DamageMultipler"), 1.0f, TEXT("Global damage multiplier in AttributeComponent."), ECVF_Cheat);


// Sets default values for this component's properties
USAttributeComponent::USAttributeComponent()
{
	Health = MaxHealth;
}

bool USAttributeComponent::IsAlive() const
{
	// returns true if alive, false if dead
	return Health > 0;
}

bool USAttributeComponent::IsFullyHealed() const
{
	return Health == MaxHealth;
}

bool USAttributeComponent::IsHealthHigherThen(float IsHealthHigherThenThis) const
{
	return Health > IsHealthHigherThenThis;
}

bool USAttributeComponent::Kill(AActor* InstigatorActor)
{
	return ApplyHealthChange(nullptr, -9999999.f);
}

bool USAttributeComponent::ApplyHealthChange(AActor* InstigatorActor, float HealthDelta)
{
	if (!GetOwner()->CanBeDamaged())
	{
		UE_LOG(LogTemp, Warning, TEXT("%s cannot be damaged!"), *GetNameSafe(GetOwner()));
		return false;
	}

	if (HealthDelta < 0.0f)
	{
		float DamageMultipler = CVarDamageMultipler.GetValueOnGameThread();

		HealthDelta *= DamageMultipler;
	}


	float OldHealth = Health;

	Health = FMath::Clamp(Health + HealthDelta, 0.0f, MaxHealth);

	float ActualDeltaHealth = Health - OldHealth;
	OnHealthChanged.Broadcast(InstigatorActor, this, Health, ActualDeltaHealth);

	// Actor just died
	if (ActualDeltaHealth < 0.0f && Health == 0.0f)
	{
		TObjectPtr<ASGameModeBase> GM = GetWorld()->GetAuthGameMode<ASGameModeBase>();
		GM->OnActorKilled(GetOwner(), InstigatorActor);
	}

	// Returns false if dealth 0 damage (actor is already dead)
	return ActualDeltaHealth != 0;
}

USAttributeComponent* USAttributeComponent::GetAttributeComponent(AActor* FromActor)
{
	if (FromActor)
	{
		return Cast<USAttributeComponent>(FromActor->GetComponentByClass(USAttributeComponent::StaticClass()));
	}

	return nullptr;
}

bool USAttributeComponent::IsActorAlive(AActor* Actor)
{
	TObjectPtr<USAttributeComponent> AttributeCompont =	GetAttributeComponent(Actor);
	if (AttributeCompont)
	{
		return AttributeCompont->IsAlive();
	}

	return false;
}