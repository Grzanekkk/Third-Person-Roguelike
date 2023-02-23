// Made by Jan Puto 2022 :D

#include "Projectiles/SMagicProjectile.h"
#include "Actions/Effects/SActionEffect.h"
#include "Characters/SCharacter.h"
#include "Components/CapsuleComponent.h"
#include "Components/SActionComponent.h"
#include "Components/SAttributeComponent.h"
#include "Components/SphereComponent.h"
#include "FunctionLibrary/GameplayFunctionLibrary.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Enums/SEnums_Logs.h"
#include "FunctionLibrary/LogsFunctionLibrary.h"

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

void ASMagicProjectile::OnComponentHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
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

		if (UGameplayFunctionLibrary::ApplyDirectionalDamage(GetInstigator(), OtherActor, Damage, Hit) && ensureAlways(BurningActionClass))
		{
			if (GetOwner())
			{
				if (ActionComp && GetOwner()->HasAuthority())
				{
					ActionComp->AddAction(GetInstigator(), BurningActionClass);
				}
			}
			else
			{
				FString Msg = FString::Printf(TEXT("This projectile has no owner!"));
				ULogsFunctionLibrary::LogOnScreen(GetWorld(), Msg, ERogueLogCategory::ERROR);
			}
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