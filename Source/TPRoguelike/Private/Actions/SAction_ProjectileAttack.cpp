// Made by Jan Puto 2022 :D


#include "Actions/SAction_ProjectileAttack.h"
#include "GameFramework/Character.h"
#include "Projectiles/SProjectileBase.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"


USAction_ProjectileAttack::USAction_ProjectileAttack()
{
	WeaponSocketName = TEXT("RightHandSocket");
	AttackAnimDelay = 0.15f;
}

void USAction_ProjectileAttack::StartAction_Implementation(AActor* Instigator)
{
	Super::StartAction_Implementation(Instigator);

	TObjectPtr<ACharacter> InstigatorCharacter = Cast<ACharacter>(Instigator);
	if (ensureMsgf(InstigatorCharacter, TEXT("Trying to use USAction_ProjectileAttack not from a character!")))
	{
		// Delay before attack (we are waiting for an animation)
		InstigatorCharacter->PlayAnimMontage(AttackAnim);

		FTimerHandle Attack_TimerHandle;
		FTimerDelegate Attack_Delegate;

		Attack_Delegate.BindUFunction(this, "AtackDelay_Elapsed", InstigatorCharacter);

		InstigatorCharacter->GetWorldTimerManager().SetTimer(Attack_TimerHandle, Attack_Delegate, AttackAnimDelay, false);
	}
}


void USAction_ProjectileAttack::AtackDelay_Elapsed(ACharacter* InstigatorCharacter)
{
	if (ensureMsgf(Attack_ProjectileClass, TEXT("Attack_ProjectileClass is missing! Please assign Attack_ProjectileClass in your SAction")))
	{
		FVector TraceStart = FVector::ZeroVector;
		FRotator EyeRotation;
		InstigatorCharacter->GetController()->GetPlayerViewPoint(TraceStart, EyeRotation); // @fixme fix this for AI
		FVector ShotDirection = EyeRotation.Vector();
		FVector TraceEnd = TraceStart + ShotDirection * 5000;

		FHitResult HitResult;
		bool bHitSuccess = GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ECC_WorldStatic);

		if (bHitSuccess)
		{
			TraceEnd = HitResult.ImpactPoint;
		}

		FVector HandLocation = InstigatorCharacter->GetMesh()->GetSocketLocation(WeaponSocketName);
		FRotator ProjectileSpawnRotation = UKismetMathLibrary::FindLookAtRotation(HandLocation, TraceEnd);

		// SpawnTM == SpawnTransformMatrix
		FTransform SpawnTM = FTransform(ProjectileSpawnRotation, HandLocation);

		TObjectPtr<ASProjectileBase> ProjectileSpawned = GetWorld()->SpawnActorDeferred<ASProjectileBase>(Attack_ProjectileClass, SpawnTM, InstigatorCharacter, InstigatorCharacter, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
		ProjectileSpawned->SetInstigator(InstigatorCharacter);
		ProjectileSpawned->FinishSpawning(SpawnTM);

		// Spawn FX
		UGameplayStatics::SpawnEmitterAttached(ProjectileSpawned->SpawnVFX, InstigatorCharacter->GetMesh(), WeaponSocketName, FVector::ZeroVector, FRotator::ZeroRotator, EAttachLocation::SnapToTarget);

		DrawDebugSphere(GetWorld(), SpawnTM.GetLocation(), 3.f, 8, FColor::Purple, false, 2.f);
		DrawDebugSphere(GetWorld(), TraceEnd, 3.f, 8, FColor::Blue, true, 2.f);
	}

	StopAction(InstigatorCharacter);
}