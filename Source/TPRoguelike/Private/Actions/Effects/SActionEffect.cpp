// Made by Jan Puto 2022 :D


#include "Actions/Effects/SActionEffect.h"
#include "Components/SActionComponent.h"

USActionEffect::USActionEffect()
{
	bAutoStart = true;
}

void USActionEffect::StartAction_Implementation(AActor* Instigator)
{
	Super::StartAction_Implementation(Instigator);

	if (Duration > 0.0f)
	{
		FTimerDelegate DurationDelegate;
		DurationDelegate.BindUFunction(this, "StopAction", Instigator);

		GetWorld()->GetTimerManager().SetTimer(DurationTimerHandle, DurationDelegate, Duration, false);
	}

	if (Period > 0.0f)
	{
		FTimerDelegate PeriodDelegate;
		PeriodDelegate.BindUFunction(this, "ExecutePeriodicEffect", Instigator);

		GetWorld()->GetTimerManager().SetTimer(PeriodTimerHandle, PeriodDelegate, Period, true);
	}
}

void USActionEffect::StopAction_Implementation(AActor* Instigator)
{
	if (GetWorld()->GetTimerManager().GetTimerRemaining(PeriodTimerHandle) < KINDA_SMALL_NUMBER)
	{
		ExecutePeriodicEffect(Instigator);
	}

	Super::StopAction_Implementation(Instigator);

	GetWorld()->GetTimerManager().ClearTimer(DurationTimerHandle);
	GetWorld()->GetTimerManager().ClearTimer(PeriodTimerHandle);

	TObjectPtr<USActionComponent> ActionComp = GetOuterComponent();
	if (ActionComp)
	{
		// should be garbage colected after this
		ActionComp->RemoveAction(this);
	}
}

void USActionEffect::ExecutePeriodicEffect_Implementation(AActor* Instigator)
{
	// Implemented in children
}
