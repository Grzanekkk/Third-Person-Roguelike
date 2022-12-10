// Made by Jan Puto 2022 :D


#include "Components/SInteractionComponent.h"
#include "DrawDebugHelpers.h"
#include "SGameplayInterface.h"

static TAutoConsoleVariable<bool> CVarDrawInteractionDebug(TEXT("jp.DrawInteractionDebug"), false, TEXT("Draw Debug Shapes for Interaction Component."), ECVF_Cheat);


// Sets default values for this component's properties
USInteractionComponent::USInteractionComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}

// Called when the game starts
void USInteractionComponent::BeginPlay()
{
	Super::BeginPlay();

}


// Called every frame
void USInteractionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

void USInteractionComponent::PrimaryInteract()
{
	bool bDrawDebug = CVarDrawInteractionDebug.GetValueOnGameThread();

	AActor* Owner = GetOwner();

	FVector TraceStart = FVector::ZeroVector;
	FRotator EyeRotation;
	Owner->GetInstigatorController()->GetPlayerViewPoint(TraceStart, EyeRotation);
	FVector ShotDirection = EyeRotation.Vector();
	FVector TraceEnd = TraceStart + ShotDirection * 1000;

	FCollisionObjectQueryParams ObjectQueryParams;
	ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldDynamic);
	
	float Radius = 30.f;
	
	TArray<FHitResult> Hits;
	FCollisionShape SweepShape;
	SweepShape.SetSphere(Radius);
	
	bool bHitSuccess = GetWorld()->SweepMultiByObjectType(Hits, TraceStart, TraceEnd, FQuat::Identity, ObjectQueryParams, SweepShape);
	FColor LineColor = bHitSuccess ? FColor::Green : FColor::Red;

	for(FHitResult Hit : Hits)
	{
		if (bDrawDebug)
		{
			DrawDebugSphere(GetWorld(), Hit.ImpactNormal, Radius, 32, LineColor, false, 2.f);
		}

		AActor* HitActor = Hit.GetActor();
		if(HitActor)
		{
			if(HitActor->Implements<USGameplayInterface>())
			{
				APawn* OwnerPawn = Cast<APawn>(Owner);
			
				if (ISGameplayInterface::Execute_CanInteract(HitActor, OwnerPawn))
				{
					ISGameplayInterface::Execute_Interact(HitActor, OwnerPawn);
				}
				break;
			}
			
		}
	}
	
	if (bDrawDebug)
	{
		DrawDebugLine(GetWorld(), TraceStart, TraceEnd, LineColor, false, 2, 0, 2);
	}
}

