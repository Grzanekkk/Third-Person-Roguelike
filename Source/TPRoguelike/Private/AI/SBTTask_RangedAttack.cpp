// Made by Jan Puto 2022 :D


#include "AI/SBTTask_RangedAttack.h"
#include "AIController.h"
#include "GameFramework/Character.h"
#include "Components/SAttributeComponent.h"
#include "Projectiles/SProjectileBase.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "FunctionLibrary/LogsFunctionLibrary.h"

EBTNodeResult::Type USBTTask_RangedAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	TObjectPtr<AAIController> OwnerController = OwnerComp.GetAIOwner();
	if (ensure(OwnerController))
	{
		TObjectPtr<ACharacter> AIPawn = Cast<ACharacter>(OwnerController->GetPawn());
		if (AIPawn == nullptr)
		{
			return EBTNodeResult::Failed;
		}

		TObjectPtr<AActor> TargetActor = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject("TargetActor"));
		if (TargetActor == nullptr)
		{
			ULogsFunctionLibrary::LogToOutputLog(GetWorld(), "There is no TargetActor", ERogueLogCategory::WARNING);
			return EBTNodeResult::Failed;
		}

		if (!USAttributeComponent::IsActorAlive(TargetActor))
		{
			ULogsFunctionLibrary::LogToOutputLog(GetWorld(), "TargetActor is Dead", ERogueLogCategory::WARNING);
			return EBTNodeResult::Failed;
		}

		FVector MuzzleLocation = AIPawn->GetMesh()->GetSocketLocation("Muzzle_Front");
		FVector Direction = TargetActor->GetActorLocation() - MuzzleLocation;
		FRotator MuzzleRotation = Direction.Rotation();

		MuzzleRotation.Pitch += FMath::RandRange(-0.2f, MaxBulletSpread);
		MuzzleRotation.Yaw += FMath::RandRange(-MaxBulletSpread, MaxBulletSpread);

		FTransform SpawnTM;
		SpawnTM.SetLocation(MuzzleLocation);
		SpawnTM.SetRotation(MuzzleRotation.Quaternion());

		TObjectPtr<AActor> NewProjectile = GetWorld()->SpawnActorDeferred<ASProjectileBase>(ProjectileClass, SpawnTM, AIPawn, AIPawn, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
		NewProjectile->SetInstigator(AIPawn);
		NewProjectile->FinishSpawning(SpawnTM);

		return NewProjectile ? EBTNodeResult::Succeeded : EBTNodeResult::Failed;
	}

	return EBTNodeResult::Failed;
}