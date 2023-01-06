// Made by Jan Puto 2022 :D


#include "Pickups/SPickupBase.h"
#include "Kismet/GameplayStatics.h"
#include "Components/SphereComponent.h"
#include "PlayerStates/SPlayerState.h"
#include "Particles/ParticleSystemComponent.h"
#include "Net/UnrealNetwork.h"

// Sets default values
ASPickupBase::ASPickupBase() 
{
	PrimaryActorTick.bCanEverTick = true;

	RootSphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("RootComponent"));
	RootSphereComp->SetSphereRadius(0.f);
	RootSphereComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	RootComponent = RootSphereComp;

	CollisionSphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	CollisionSphereComp->SetSphereRadius(64.f);
	CollisionSphereComp->SetupAttachment(RootComponent);

	MainMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MainMesh"));
	MainMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	MainMeshComponent->SetupAttachment(CollisionSphereComp);

	IdleParticleComponent = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("IdleParticleComponent"));
	IdleParticleComponent->bAutoActivate = true;
	IdleParticleComponent->SetupAttachment(CollisionSphereComp);

	SetReplicates(true);
}


void ASPickupBase::BeginPlay()
{
	Super::BeginPlay();
	
}


void ASPickupBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


void ASPickupBase::Interact_Implementation(APawn* InstigatorPawn)
{
	//UsePickupItem(InstigatorPawn);
	MulticastUsePickupItem(InstigatorPawn);
}


bool ASPickupBase::CanInteract_Implementation(APawn* InstigatorPawn)
{
	if (IsEnabled_Implementation())
	{
		if (InstigatorPawn->GetPlayerState<ASPlayerState>()->ApplyCreditsChange(-UseCost))
		{
			return true;
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("You dont have enaugh Credits to use this pickup"));
			return false;
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Cannot interact right now"));
		return false;
	}
}


bool ASPickupBase::IsEnabled_Implementation()
{
	return IsEnabled;
}


void ASPickupBase::UsePickupItem(APawn* InstigatorPawn)
{
	if (InstigatorPawn->HasAuthority())
	{
		IsEnabled = false;

		// Its here so stuff inside will also get executed on the server
		OnRep_IsEnabled();

		GetWorldTimerManager().SetTimer(InteractionDelay_TimerHandle, this, &ASPickupBase::AllowInteraction, InteractionDelay);
	}
}


void ASPickupBase::MulticastUsePickupItem_Implementation(APawn* InstigatorPawn)
{
	UsePickupItem(InstigatorPawn);
}


void ASPickupBase::AllowInteraction()
{
	IsEnabled = true;

	OnRep_IsEnabled();
}


void ASPickupBase::OnRep_IsEnabled()
{
	if (IsEnabled)
	{
		// We just enabled this item
		RootComponent->SetVisibility(IsEnabled, true);
		IdleParticleComponent->Activate();
	}
	else
	{
		// We just picked up this item
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), PickUpParticles, GetActorLocation());

		RootComponent->SetVisibility(IsEnabled, true);
		IdleParticleComponent->Deactivate();
	}
}

void ASPickupBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ASPickupBase, IsEnabled);
}