// Made by Jan Puto 2022 :D

#include "Projectiles/SProjectileBase.h"
#include "Components/SphereComponent.h"
#include "Components/AudioComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"
#include "Sound/SoundCue.h"

// Sets default values
ASProjectileBase::ASProjectileBase()
{
	SphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	SphereComp->SetCollisionProfileName("Projectile");
	//SphereComp->SetCollisionObjectType(ECC_EngineTraceChannel1); // Projectile
	RootComponent = SphereComp;

	ParticleComp = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ParticleComp"));
	ParticleComp->SetupAttachment(RootComponent);

	MovementComp = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
	MovementComp->bRotationFollowsVelocity = true;
	MovementComp->bInitialVelocityInLocalSpace = true;
	MovementComp->InitialSpeed = 1000.f;
	MovementComp->ProjectileGravityScale = 0.0f;

	AudioComp = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioComp"));
	AudioComp->SetupAttachment(RootComponent);
}

void ASProjectileBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	//SphereComp->OnComponentHit.AddDynamic(this, &ASProjectileBase::OnComponentHit);
	SphereComp->OnComponentBeginOverlap.AddDynamic(this, &ASProjectileBase::OnActorOverlap);
}

void ASProjectileBase::BeginPlay()
{
	Super::BeginPlay();

	AudioComp->SetSound(FlightSound);
	AudioComp->Play();

	AActor* InstigatorActor = Cast<AActor>(GetInstigator());
	SphereComp->IgnoreActorWhenMoving(InstigatorActor, true);

	// Its not working
	//// We can set this value either in defaults or when spawning actor deffered
	//MovementComp->InitialSpeed = ProjectileSpeed;
	//UE_LOG(LogTemp, Warning, TEXT("Projectile Speed: %f"), MovementComp->InitialSpeed);

	if (ensureMsgf(GetInstigator(), TEXT("Instigator is missing in your projectile")))
	{
		SphereComp->IgnoreActorWhenMoving(GetInstigator(), true);
		GetInstigator()->MoveIgnoreActorAdd(this);
	}
}

void ASProjectileBase::OnComponentHit(UPrimitiveComponent* HitComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	//if(OtherActor == Cast<AActor>(GetInstigator()))
	//	return;
	//
	//Explode();
}

void ASProjectileBase::OnActorOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//UE_LOG(LogTemp, Warning, TEXT("Hit Actor: %s, Component: %s"), *GetNameSafe(OtherActor), *GetNameSafe(OverlappedComponent));
	if (OtherActor && OtherActor != GetInstigator() && !OtherActor->IsA(ASProjectileBase::StaticClass()))
	{
		Explode();
	}
}

void ASProjectileBase::Explode()
{
	if(ensure(!IsPendingKill()))
	{
		UGameplayStatics::SpawnEmitterAtLocation(this, ImpactVFX, GetActorLocation(), GetActorRotation());
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), ImpactSound, GetActorLocation());
		UGameplayStatics::PlayWorldCameraShake(GetWorld(), ImpactCameraShake, GetActorLocation(), 10.f, 600.f);
	}
}