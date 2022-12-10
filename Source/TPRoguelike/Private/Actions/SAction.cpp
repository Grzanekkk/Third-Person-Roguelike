// Made by Jan Puto 2022 :D


#include "Actions/SAction.h"
#include "Components/SActionComponent.h"

bool USAction::CanStart_Implementation(AActor* Instigator)
{
	if (IsRunning())
	{
		return false;
	}

	TObjectPtr<USActionComponent> Comp = GetOuterComponent();
	if (Comp->ActiveGameplayTags.HasAny(BlockedTags))
	{
		return false;
	}

	return true;
}

void USAction::StartAction_Implementation(AActor* Instigator)
{
	UE_LOG(LogTemp, Log, TEXT("Running: %s"), *GetNameSafe(this));
	TObjectPtr<USActionComponent> Comp = GetOuterComponent();
	Comp->ActiveGameplayTags.AppendTags(GrantsTags);

	bIsRunning = true;
}

void USAction::StopAction_Implementation(AActor* Instigator)
{
	UE_LOG(LogTemp, Log, TEXT("Stopping: %s"), *GetNameSafe(this));

	ensureAlways(bIsRunning); // If this is false here this means that we are stopping this action for a seound time and something is not right

	TObjectPtr<USActionComponent> Comp = GetOuterComponent();
	Comp->ActiveGameplayTags.RemoveTags(GrantsTags);

	bIsRunning = false;
}

UWorld* USAction::GetWorld() const
{
	// Outer is set when creating actions via NewObject<T>
	// Outer == ActionComponent on Actor
	TObjectPtr<USActionComponent> Comp = GetOuterComponent();
	if (Comp)
	{
		return Comp->GetWorld();
	}

	return nullptr;
}

USActionComponent* USAction::GetOuterComponent() const
{
	// Should always return valid component unless we really fucked up
	return Cast<USActionComponent>(GetOuter());
}
