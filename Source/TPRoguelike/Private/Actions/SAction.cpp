// Made by Jan Puto 2022 :D


#include "Actions/SAction.h"
#include "Components/SActionComponent.h"
#include "FunctionLibrary/LogsFunctionLibrary.h"
#include "Net/UnrealNetwork.h"

void USAction::Initialize(USActionComponent* NewActionComponent)
{
	OwningActionComponent = NewActionComponent;
}

bool USAction::CanStart_Implementation(AActor* Instigator)
{
	if (IsRunning())
	{
		return false;
	}

	TObjectPtr<USActionComponent> Comp = GetOwningComponent();
	if (Comp->ActiveGameplayTags.HasAny(BlockedTags))
	{
		return false;
	}

	return true;
}

void USAction::StartAction_Implementation(AActor* Instigator)
{
	//UE_LOG(LogTemp, Log, TEXT("Running: %s"), *GetNameSafe(this));
	//ULogsFunctionLibrary::LogOnScreen_IsClientServer(this, FString::Printf(TEXT("Started: %s"), *ActionName.ToString()), FColor::Green);

	TObjectPtr<USActionComponent> Comp = GetOwningComponent();
	Comp->ActiveGameplayTags.AppendTags(GrantsTags);

	ReplicationData.bIsRunning = true;
	ReplicationData.Instigator = Instigator;
}

void USAction::StopAction_Implementation(AActor* Instigator)
{
	//UE_LOG(LogTemp, Log, TEXT("Stopping: %s"), *GetNameSafe(this));
	//ULogsFunctionLibrary::LogOnScreen_IsClientServer(this, FString::Printf(TEXT("Stopped: %s"), *ActionName.ToString()), FColor::White);

	//ensureAlways(bIsRunning); // If this is false here this means that we are stopping this action for a seound time and something is not right

	TObjectPtr<USActionComponent> Comp = GetOwningComponent();
	Comp->ActiveGameplayTags.RemoveTags(GrantsTags);

	ReplicationData.bIsRunning = false;
	ReplicationData.Instigator = Instigator;
}

UWorld* USAction::GetWorld() const
{
	// Outer is set when creating actions via NewObject<T>
	// Outer == ActionComponent on Actor
	TObjectPtr<AActor> Actor = Cast<AActor>(GetOuter());
	if (Actor)
	{
		return Actor->GetWorld();
	}

	return nullptr;
}

USActionComponent* USAction::GetOwningComponent() const
{
	// Should always return valid component unless we really fucked up
	return OwningActionComponent;
}

void USAction::OnRep_ReplicatoinData()
{
	//ULogsFunctionLibrary::LogOnScreen_IsClientServer(this, FString::Printf(TEXT("Running OnRep_IsRunning() -> %b"), ReplicationData.bIsRunning), FColor::Cyan);

	if (ReplicationData.bIsRunning)
	{
		StartAction(ReplicationData.Instigator);
	}
	else
	{
		StopAction(ReplicationData.Instigator);
	}
}

bool USAction::IsSupportedForNetworking() const
{
	return true;
}

void USAction::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(USAction, ReplicationData);
	DOREPLIFETIME(USAction, OwningActionComponent);
}