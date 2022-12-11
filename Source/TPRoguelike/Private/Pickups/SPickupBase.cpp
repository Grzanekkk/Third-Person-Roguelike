// Made by Jan Puto 2022 :D


#include "Pickups/SPickupBase.h"
#include "Kismet/GameplayStatics.h"
#include "Components/SphereComponent.h"
#include "PlayerStates/SPlayerState.h"
#include "Particles/ParticleSystemComponent.h"

// Sets default values
ASPickupBase::ASPickupBase() 
{
	PrimaryActorTick.bCanEverTick = true;

	CollisionSphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	CollisionSphereComp->SetSphereRadius(64.f);
	RootComponent = CollisionSphereComp;

	MainMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MainMesh"));
	MainMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	MainMeshComponent->SetupAttachment(RootComponent);

	IdleParticleComponent = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("IdleParticleComponent"));
	IdleParticleComponent->SetupAttachment(RootComponent);
	IdleParticleComponent->bAutoActivate = true;
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
	UsePickupItem(InstigatorPawn);
}

bool ASPickupBase::CanInteract_Implementation(APawn* InstigatorPawn)
{
	if (bCanBeInteracted)
	{
		if (InstigatorPawn->GetPlayerState<ASPlayerState>()->AddCredits(-UseCost))
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

void ASPickupBase::UsePickupItem(APawn* InstigatorPawn)
{
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), PickUpParticles, GetActorLocation());
	bCanBeInteracted = false;
	RootComponent->SetVisibility(bCanBeInteracted, true);
	IdleParticleComponent->Deactivate();

	GetWorldTimerManager().SetTimer(InteractionDelay_TimerHandle, this, &ASPickupBase::AllowInteraction, InteractionDelay);
}

void ASPickupBase::AllowInteraction()
{
	bCanBeInteracted = true;
	RootComponent->SetVisibility(bCanBeInteracted, true);
	IdleParticleComponent->Activate();
}