// Made by Jan Puto 2022 :D

#include "Projectiles/SDashProjectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Kismet/GameplayStatics.h"

ASDashProjectile::ASDashProjectile()
{

}

void ASDashProjectile::BeginPlay()
{
	Super::BeginPlay();

	TargetActor = GetInstigator();

	GetWorldTimerManager().SetTimer(ExpolsionTimer, this, &ASDashProjectile::Explode, ExplodeAfter);
}

void ASDashProjectile::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

void ASDashProjectile::OnComponentHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	//Super::OnComponentHit(HitComponent, OtherActor, OtherComp, NormalImpulse, Hit);

}

void ASDashProjectile::OnActorOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Warning, TEXT("Hit Actor: %s, Component: %s"), *GetNameSafe(OtherActor), *GetNameSafe(OverlappedComponent));

	if (OtherActor && OtherActor != GetInstigator() && !OtherActor->IsA(ASProjectileBase::StaticClass()))
	{
		GetWorldTimerManager().ClearTimer(ExpolsionTimer);

		Explode();
	}
}

void ASDashProjectile::Explode()
{
	GetWorldTimerManager().SetTimer(TeleportationTimer, this, &ASDashProjectile::TeleportPlayer, TeleportDelay);

	UGameplayStatics::SpawnEmitterAtLocation(this, PlayerTeleportVFX, GetInstigator()->GetActorLocation(), GetInstigator()->GetActorRotation());

	MovementComp->StopMovementImmediately();
	ParticleComp->Deactivate();
}

void ASDashProjectile::TeleportPlayer()
{
	Super::Explode();

	if (ensureMsgf(TargetActor, TEXT("TargetActor is null in Dash Projectile! Aborting teleportation")))
	{
		TargetActor->TeleportTo(GetActorLocation(), TargetActor->GetActorRotation());
	}

	Destroy();
}
