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
	if (IsEnabled_Implementation())
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

bool ASPickupBase::IsEnabled_Implementation()
{
	return IsEnabled;
}

void ASPickupBase::UsePickupItem(APawn* InstigatorPawn)
{
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), PickUpParticles, GetActorLocation());
	IsEnabled = false;
	RootComponent->SetVisibility(IsEnabled, true);
	IdleParticleComponent->Deactivate();

	GetWorldTimerManager().SetTimer(InteractionDelay_TimerHandle, this, &ASPickupBase::AllowInteraction, InteractionDelay);
}

void ASPickupBase::AllowInteraction()
{
	IsEnabled = true;
	RootComponent->SetVisibility(IsEnabled, true);
	IdleParticleComponent->Activate();
}