// Made by Jan Puto 2022 :D


#include "Components/SInteractionComponent.h"
#include "DrawDebugHelpers.h"
#include "SGameplayInterface.h"
#include "UI/SWorldUserWidget.h"

static TAutoConsoleVariable<bool> CVarDrawInteractionDebug(TEXT("jp.DrawInteractionDebug"), false, TEXT("Draw Debug Shapes for Interaction Component."), ECVF_Cheat);


USInteractionComponent::USInteractionComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}

void USInteractionComponent::BeginPlay()
{
	Super::BeginPlay();

}

void USInteractionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	APawn* OwnerPawn = Cast<APawn>(GetOwner());
	if (OwnerPawn->IsLocallyControlled())
	{
		FindBestInteractable();
	}
}

void USInteractionComponent::FindBestInteractable()
{
	bool bDrawDebug = CVarDrawInteractionDebug.GetValueOnGameThread();

	AActor* Owner = GetOwner();
	if (Owner == nullptr || Owner->GetInstigatorController() == nullptr)
	{
		return;
	}

	FVector TraceStart = FVector::ZeroVector;
	FRotator EyeRotation;
	Owner->GetInstigatorController()->GetPlayerViewPoint(TraceStart, EyeRotation);
	FVector ShotDirection = EyeRotation.Vector();
	FVector TraceEnd = TraceStart + ShotDirection * InteractionDistance;

	FCollisionObjectQueryParams ObjectQueryParams;
	ObjectQueryParams.AddObjectTypesToQuery(CollisionChannel);

	TArray<FHitResult> Hits;
	FCollisionShape SweepShape;
	SweepShape.SetSphere(InteractionRadius);

	bool bHitSuccess = GetWorld()->SweepMultiByObjectType(Hits, TraceStart, TraceEnd, FQuat::Identity, ObjectQueryParams, SweepShape);
	FColor LineColor = bHitSuccess ? FColor::Green : FColor::Red;

	// Clear ref
	FocusedActor = nullptr;
	APawn* OwnerPawn = Cast<APawn>(GetOwner());

	for (FHitResult Hit : Hits)
	{
		if (bDrawDebug)
		{
			DrawDebugSphere(GetWorld(), Hit.ImpactNormal, InteractionRadius, 32, LineColor, false, 0.f);
		}

		AActor* HitActor = Hit.GetActor();
		if (HitActor)
		{
			if (HitActor->Implements<USGameplayInterface>() && ISGameplayInterface::Execute_IsEnabled(HitActor))
			{
				FocusedActor = HitActor;
				break;
			}
		}
	}

	if (FocusedActor)
	{
		if (WidgetInstance == nullptr && ensure(DefaultWidgetClass))
		{
			WidgetInstance = CreateWidget<USWorldUserWidget>(GetWorld(), DefaultWidgetClass);
		}

		if (WidgetInstance)
		{
			WidgetInstance->AttachedActor = FocusedActor;

			if (!WidgetInstance->IsInViewport())
			{
				WidgetInstance->AddToViewport();
			}
		}
	}
	else
	{
		if (WidgetInstance)
		{
			WidgetInstance->RemoveFromParent();
		}
	}

	if (bDrawDebug)
	{
		DrawDebugLine(GetWorld(), TraceStart, TraceEnd, LineColor, false, 2, 0, 0);
	}
}


void USInteractionComponent::PrimaryInteract()
{
	ServerPrimaryInteract(FocusedActor);
}

void USInteractionComponent::ServerPrimaryInteract_Implementation(AActor* InFocusedActor)
{
	if (InFocusedActor == nullptr)
	{
		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, "No Focused Actreo to Interact.");
		return;
	}

	APawn* OwnerPawn = Cast<APawn>(GetOwner());

	if (ISGameplayInterface::Execute_CanInteract(InFocusedActor, OwnerPawn))
	{
		ISGameplayInterface::Execute_Interact(InFocusedActor, OwnerPawn);
	}
}