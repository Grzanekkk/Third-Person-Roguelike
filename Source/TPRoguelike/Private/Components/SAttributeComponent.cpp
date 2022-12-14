// Made by Jan Puto 2022 :D


#include "Components/SAttributeComponent.h"
#include "GameModes/SGameModeBase.h"
#include "Net/UnrealNetwork.h"


static TAutoConsoleVariable<float> CVarDamageMultipler(TEXT("jp.DamageMultipler"), 1.0f, TEXT("Global damage multiplier in AttributeComponent."), ECVF_Cheat);


// Sets default values for this component's properties
USAttributeComponent::USAttributeComponent()
{
	CurrentHealth = MaxHealth;

	SetIsReplicatedByDefault(true);
}

bool USAttributeComponent::IsAlive() const
{
	// returns true if alive, false if dead
	return CurrentHealth > 0;
}

bool USAttributeComponent::IsFullyHealed() const
{
	return CurrentHealth == MaxHealth;
}

bool USAttributeComponent::IsHealthHigherThen(float IsHealthHigherThenThis) const
{
	return CurrentHealth > IsHealthHigherThenThis;
}

bool USAttributeComponent::Kill(AActor* InstigatorActor)
{
	return ApplyHealthChange(nullptr, -9999999.f);
}

// If HealthDealta is < 0 then we are healing
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

	// Calculating Health
	float OldHealth = CurrentHealth;
	float NewHealth = FMath::Clamp(CurrentHealth + HealthDelta, 0.0f, MaxHealth);
	float ActualDeltaHealth = NewHealth - OldHealth;

	// Is Server?
	if (GetOwner()->HasAuthority())
	{
		CurrentHealth = NewHealth;

		// Actor just died
		if (ActualDeltaHealth < 0.0f && CurrentHealth == 0.0f)
		{
			TObjectPtr<ASGameModeBase> GM = GetWorld()->GetAuthGameMode<ASGameModeBase>();
			GM->OnActorKilled(GetOwner(), InstigatorActor);
		}
	}

	// if this multicast is called on the client it will be called like a reguilar function
	MulticastOnHealthChanged(InstigatorActor, this, NewHealth, ActualDeltaHealth);

	// We dont want to lose rage when we are getting healed
	if (ActualDeltaHealth < 0.0f)
	{
		float RageToApply = (-ActualDeltaHealth * RageMultiplier);
		ApplyRageChange(InstigatorActor, RageToApply);
	}

	// Returns false if dealth 0 damage (actor is already dead)
	return ActualDeltaHealth != 0;
}

bool USAttributeComponent::ApplyRageChange(AActor* InstigatorActor, float RageDelta)
{
	// If we and spend rage we return false
	if (RageDelta == 0.0f || CurrentRage + RageDelta < 0.0f)
	{
		return false;
	}

	// Calculating Rage
	float OldRage = CurrentRage;
	CurrentRage = FMath::Clamp(OldRage + RageDelta, 0.0f, MaxRage);

	float ActualRageDelta = CurrentRage - OldRage;
	OnRageChanged.Broadcast(InstigatorActor, this, CurrentRage, ActualRageDelta);

	return true;
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

void USAttributeComponent::MulticastOnHealthChanged_Implementation(AActor* InstigatorActor, USAttributeComponent* OwningComp, float NewHealth, float HealthDelta)
{
	OnHealthChanged.Broadcast(InstigatorActor, this, NewHealth, HealthDelta);
}

void USAttributeComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(USAttributeComponent, CurrentHealth);
	DOREPLIFETIME(USAttributeComponent, MaxHealth);
}