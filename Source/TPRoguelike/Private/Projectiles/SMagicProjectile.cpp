// Made by Jan Puto 2022 :D

#include "Projectiles/SMagicProjectile.h"
#include "Components/SAttributeComponent.h"
#include "Components/SphereComponent.h"
#include "Components/SActionComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Characters/SCharacter.h"
#include "Components/CapsuleComponent.h"
#include "FunctionLibrary/GameplayFunctionLibrary.h"
#include "Actions/Effects/SActionEffect.h"

// Sets default values
ASMagicProjectile::ASMagicProjectile()
{
	Damage = 20.f;
}

void ASMagicProjectile::BeginPlay()
{
	Super::BeginPlay();
	SphereComp->IgnoreActorWhenMoving(GetInstigator(), true);
}

void ASMagicProjectile::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

void ASMagicProjectile::OnActorOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor != GetInstigator() && !OtherActor->IsA(ASProjectileBase::StaticClass()))
	{
		TObjectPtr<USActionComponent> ActionComp = Cast<USActionComponent>(OtherActor->GetComponentByClass(USActionComponent::StaticClass()));
		if (ActionComp && ActionComp->ActiveGameplayTags.HasTag(ParryTag))
		{
			MovementComp->Velocity = -MovementComp->Velocity;

			SetInstigator(Cast<APawn>(OtherActor));
			return;
		}

		if (UGameplayFunctionLibrary::ApplyDirectionalDamage(GetInstigator(), OtherActor, Damage, SweepResult) && ensure(BurningActionClass))
		{
			ActionComp->AddAction(GetInstigator(), BurningActionClass);
		}

		//UE_LOG(LogTemp, Warning, TEXT("Hit Actor: %s, Component: %s"), *GetNameSafe(OtherActor), *GetNameSafe(OverlappedComponent));

		Super::Explode();

		Destroy();
	}
	else
	{
		return;
	}
}
