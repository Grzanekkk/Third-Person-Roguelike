// Made by Jan Puto 2022 :D

#include "Projectiles/SDashProjectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Enums/SEnums_Logs.h"
#include "FunctionLibrary/LogsFunctionLibrary.h"


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
	Super::OnComponentHit(HitComponent, OtherActor, OtherComp, NormalImpulse, Hit);

	FString Msg = FString::Printf(TEXT("Hit Actor: %s, Component: %s"), *GetNameSafe(OtherActor), *GetNameSafe(HitComponent));
	ULogsFunctionLibrary::LogToOutputLog(GetWorld(), Msg, ERogueLogCategory::LOG);

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
