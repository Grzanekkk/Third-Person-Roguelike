// Made by Jan Puto 2022 :D


#include "Components/SActionComponent.h"
#include "Actions/SAction.h"
#include "FunctionLibrary/LogsFunctionLibrary.h"
#include "Net/UnrealNetwork.h"
#include "Engine/ActorChannel.h"

USActionComponent::USActionComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	SetIsReplicatedByDefault(true);
}

void USActionComponent::BeginPlay()
{
	Super::BeginPlay();

	// We are only adding actions on the server and then replicationg them to clients
	if (GetOwner()->HasAuthority())
	{
		for (TSubclassOf<USAction> DefaultActionClass : DefaultActions)
		{
			AddAction(GetOwner(), DefaultActionClass);
		}
	}
}

void USActionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	//FString DebugMsg = GetNameSafe(GetOwner()) + ": " + ActiveGameplayTags.ToStringSimple();
	//GEngine->AddOnScreenDebugMessage(-1, 0.0f, FColor::White, DebugMsg);

	// Draw all actions
	for (TObjectPtr<USAction> Action : Actions)
	{
		FColor TextColor = Action->IsRunning() ? FColor::Blue : FColor::White;
		FString ActionMsg = FString::Printf(TEXT("[%s] Action: %s : IsRunning: %s : Outer: %s"),
			*GetNameSafe(GetOwner()),
			*Action->ActionName.ToString(),
			Action->IsRunning() ? TEXT("true") : TEXT("false"),
			*GetNameSafe(Action->GetOuter()));
		ULogsFunctionLibrary::LogOnScreen_IsClientServer(GetOwner(), ActionMsg, TextColor, 0.0f);
	}
}

void USActionComponent::AddAction(AActor* Instigator, TSubclassOf<USAction> ActionClass)
{
	if (!ensureAlways(ActionClass))
	{
		return;
	}

	TObjectPtr<USAction> NewAction = NewObject<USAction>(GetOwner(), ActionClass);
	if (NewAction)
	{
		NewAction->Initialize(this);

		Actions.Add(NewAction);

		if (NewAction->bAutoStart && ensure(NewAction->CanStart(Instigator)))
		{
			NewAction->StartAction(Instigator);
		}
	}
}

void USActionComponent::RemoveAction(USAction* Action)
{
	if (!ensureAlways(Action && !Action->IsRunning()))
	{
		return;
	}

	Actions.Remove(Action);
}


bool USActionComponent::StartActionByName(AActor* Instigator, FName ActionName)
{
	for (TObjectPtr<USAction> Action : Actions)
	{
		if (Action && Action->ActionName == ActionName)
		{
			if (!Action->CanStart(Instigator))
			{
				FString DebugMsg = GetNameSafe(GetOwner()) + ": " + "Failed to run: " + GetNameSafe(Action);
				GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow, DebugMsg);
				return false;
			}

			// If we are a Client we want to tell the server to call StartAction and also run it locally
			// If we are a Server we are just running this function na ther Sever
			if (!GetOwner()->HasAuthority())
			{
				ServerStartActionByName(Instigator, ActionName);
			}

			Action->StartAction(Instigator);
			return true;
		}
	}

	return false;
}

void USActionComponent::ServerStartActionByName_Implementation(AActor* Instigator, FName ActionName)
{
	StartActionByName(Instigator, ActionName);
}

bool USActionComponent::StopActionByName(AActor* Instigator, FName ActionName)
{
	for (TObjectPtr<USAction> Action : Actions)
	{
		if (Action && Action->ActionName == ActionName)
		{
			if (Action->IsRunning())
			{
				Action->StopAction(Instigator);
				return true;
			}
		}
	}

	return false;
}

bool USActionComponent::ReplicateSubobjects(UActorChannel* Channel, FOutBunch* Bunch, FReplicationFlags* RepFlags)
{
	bool bWroteSomething = Super::ReplicateSubobjects(Channel, Bunch, RepFlags);
	for (TObjectPtr<USAction> Action : Actions)
	{
		if (Action)
		{
			// We are calling ReplicateSubobject on every Action and then we are checking if any of the action should be replicated, if yes we return true
			bWroteSomething |= Channel->ReplicateSubobject(Action, *Bunch, *RepFlags);
		}
	}

	return bWroteSomething;
}

void USActionComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(USActionComponent, Actions);
}