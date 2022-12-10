// Made by Jan Puto 2022 :D


#include "Actions/SAction_Parry.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"
#include "Components/SActionComponent.h"

USAction_Parry::USAction_Parry()
{

}

void USAction_Parry::StartAction_Implementation(AActor* Instigator)
{
	Super::StartAction_Implementation(Instigator);

	ParryParticleCompnent = UGameplayStatics::SpawnEmitterAttached(ParryFX, Instigator->GetRootComponent());

	FTimerHandle Parry_TimerHandle;
	FTimerDelegate Parry_Delegate;

	Parry_Delegate.BindUFunction(this, "StopAction", Instigator);

	Instigator->GetWorldTimerManager().SetTimer(Parry_TimerHandle, Parry_Delegate, ActionDuration, false);
}

void USAction_Parry::StopAction_Implementation(AActor* Instigator)
{
	if (IsRunning())
	{
		Super::StopAction_Implementation(Instigator);

		ParryParticleCompnent->Deactivate();
	}
}
