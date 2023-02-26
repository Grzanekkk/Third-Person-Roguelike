// Made by Jan Puto 2022 :D


#include "FunctionLibrary/GameplayFunctionLibrary.h"
#include "Components/SAttributeComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "GameState/SGameState.h"
#include "Components/SQuestManagerComponent.h"
#include "SGameplayInterface.h"

bool UGameplayFunctionLibrary::ApplyDamage(AActor* DamageCauser, AActor* TargetActor, float DamageAmount)
{
	TObjectPtr<USAttributeComponent> AttribComponent = USAttributeComponent::GetAttributeComponent(TargetActor);
	if (AttribComponent)
	{
		return AttribComponent->ApplyHealthChange(DamageCauser, -DamageAmount);
	}

	return false;
}

bool UGameplayFunctionLibrary::ApplyDirectionalDamage(AActor* DamageCauser, AActor* TargetActor, float DamageAmount, const FHitResult& HitResult)
{
	if (ApplyDamage(DamageCauser, TargetActor, DamageAmount))
	{
		TObjectPtr<UPrimitiveComponent> HitComp = HitResult.GetComponent();
		if (HitComp && HitComp->IsSimulatingPhysics(HitResult.BoneName))
		{
			// Direction = Target - Origin !
			FVector Direction = HitResult.TraceEnd - HitResult.TraceStart;
			Direction.Normalize();

			HitComp->AddImpulseAtLocation(Direction * 500000.f, HitResult.ImpactPoint, HitResult.BoneName);
		}

		return true;
	}
	return false;
}

bool UGameplayFunctionLibrary::AddObjectiveStat(const UObject* WorldContextObject, FGameplayTag ObjectiveTag, int32 Stat)
{
	check(UKismetSystemLibrary::IsServer(WorldContextObject));

	if (ObjectiveTag.IsValid())
	{
		TObjectPtr<USQuestManagerComponent> QuestManager = GetQuestManager(WorldContextObject);
		if (QuestManager)
		{
			QuestManager->ServerOnlyAddObjectiveStat(ObjectiveTag, 1);
			return true;
		}
	}

	return false;
}

bool UGameplayFunctionLibrary::IntaractIfPossible(const UObject* WorldContextObject, UObject* ObjectToInteract, APawn* Instigator)
{
	check(UKismetSystemLibrary::IsServer(WorldContextObject));

	// Instigator can be null if fe. a lever or a zone is interacting 

	if (ObjectToInteract && ObjectToInteract->Implements<USGameplayInterface>() && ISGameplayInterface::Execute_IsEnabled(ObjectToInteract))
	{
		if (ISGameplayInterface::Execute_CanInteract(ObjectToInteract, Instigator))
		{
			ISGameplayInterface::Execute_Interact(ObjectToInteract, Instigator);
			return true;
		}
	}

	return false;
}

ASGameState* UGameplayFunctionLibrary::GetSGameState(const UObject* WorldContextObject)
{
	return Cast<ASGameState>(UGameplayStatics::GetGameState(WorldContextObject));
}

USQuestManagerComponent* UGameplayFunctionLibrary::GetQuestManager(const UObject* WorldContextObject)
{
	TObjectPtr<ASGameState> GameState = GetSGameState(WorldContextObject);
	if (GameState)
	{
		return GameState->GetQuestManager();
	}

	return nullptr;
}
