// Made by Jan Puto 2022 :D


#include "Projectiles/Blackhole.h"
#include "Components/SphereComponent.h"
#include "PhysicsEngine/RadialForceComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Particles/ParticleSystemComponent.h"

// Sets default values
ABlackhole::ABlackhole()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	SuckingSphere = CreateDefaultSubobject<USphereComponent>(TEXT("SuckingSphere"));
	SuckingSphere->SetSphereRadius(2000);
	SuckingSphere->SetupAttachment(RootComponent);
	
	MovementComp->InitialSpeed = 1500.f;


	//RadialForceComp = CreateDefaultSubobject<URadialForceComponent>(TEXT("RadialForceComp"));
	//RadialForceComp->SetupAttachment(RootComponent);
	//RadialForceComp->bImpulseVelChange = true;
	//RadialForceComp->Radius = 420.f;
	//RadialForceComp->ForceStrength = 1200;
	//RadialForceComp->bIgnoreOwningActor = true;

	InitialLifeSpan = 5.f;
}

// Called when the game starts or when spawned
void ABlackhole::BeginPlay()
{
	Super::BeginPlay();
	
	SphereComp->IgnoreActorWhenMoving(GetInstigator(), true);
}

void ABlackhole::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	TArray<UPrimitiveComponent*> OverlappingObjects;
	SuckingSphere->GetOverlappingComponents(OverlappingObjects);	// Only works on objects that may be physically simulated
	
	for (UPrimitiveComponent* OverlappingObject : OverlappingObjects)
	{
		if (OverlappingObject && OverlappingObject->IsSimulatingPhysics())
		{
			OverlappingObject->AddRadialForce(GetActorLocation(), SuckingSphere->GetScaledSphereRadius(), SuckingStrength, ERadialImpulseFalloff::RIF_Constant, true);
		}
	}
}

// Stop the projectile
void ABlackhole::StartSucking(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	//MovementComp->StopMovementImmediately();
	//MovementComp->Activate(false);


	//RadialForceComp->
}