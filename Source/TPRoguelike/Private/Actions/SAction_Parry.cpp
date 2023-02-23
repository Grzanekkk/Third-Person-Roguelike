// Made by Jan Puto 2022 :D


#include "Actions/SAction_Parry.h"
#include "Components/SActionComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "Particles/ParticleSystemComponent.h"

void USAction_Parry::StartAction_Implementation(AActor* Instigator)
{
	Super::StartAction_Implementation(Instigator);

	ParryParticleCompnent = UGameplayStatics::SpawnEmitterAttached(ParryFX, Instigator->GetRootComponent());

	if (GetOwningComponent()->GetOwner()->HasAuthority())
	{
		FTimerDelegate Parry_Delegate;
		Parry_Delegate.BindUFunction(this, "StopAction", Instigator);

		Instigator->GetWorldTimerManager().SetTimer(Parry_TimerHandle, Parry_Delegate, ActionDuration, false);
	}
}

void USAction_Parry::StopAction_Implementation(AActor* Instigator)
{
	Super::StopAction_Implementation(Instigator);

	if (ParryParticleCompnent->IsActive())
	{
		ParryParticleCompnent->Deactivate();
	}
		
	if (GetOwningComponent()->GetOwner()->HasAuthority())
	{
		Instigator->GetWorldTimerManager().ClearTimer(Parry_TimerHandle);
	}
}

void USAction_Parry::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(USAction_Parry, Parry_TimerHandle);
}