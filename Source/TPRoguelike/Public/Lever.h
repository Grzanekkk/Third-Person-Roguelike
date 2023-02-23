// Made by Jan Puto 2023 :D

#pragma once

#include "CoreMinimal.h"
#include "SGameplayInterface.h"
#include "GameFramework/Actor.h"
#include "GameplayTagContainer.h"
#include "Lever.generated.h"

UCLASS()
class TPROGUELIKE_API ALever : public AActor, public ISGameplayInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALever();

	virtual void Interact_Implementation(APawn* InstigatorPawn);

	virtual bool CanInteract_Implementation(APawn* InstigatorPawn);

	virtual bool IsEnabled_Implementation();

	virtual bool CanBeInteractedByPlayerCharacter_Implementation();

protected:
	UFUNCTION()
	void SwitchLever();

	UFUNCTION()
	void OnRep_bIsSwitched();

	UPROPERTY(EditDefaultsOnly, Category = "Rogue|Components")
	TObjectPtr<UParticleSystemComponent> OnSwitchParticleComp;

	UPROPERTY(VisibleAnywhere, Category = "Rogue|Components")
	TObjectPtr<UStaticMeshComponent> BaseMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Rogue|Components")
	TObjectPtr<UStaticMeshComponent> LeverMesh;

	UPROPERTY(EditAnywhere, Category = "Rogue|Lever")
	TObjectPtr<AActor> ActorToInteractOnSwitch;

	UPROPERTY(ReplicatedUsing = "OnRep_bIsSwitched", BlueprintReadOnly, Category = "Rogue|Lever")
	bool bIsSwitched = false;

	UPROPERTY(EditAnywhere, Category = "Rogue|Lever")
	float SwitchedLeverPitch = -30.f;

	UPROPERTY(EditAnywhere, Category = "Rogue|Lever")
	float NotSwitchedLeverPitch = 30.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Rogue|Lever|Quest")
	FGameplayTag ObjectiveTag;
};
