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

	virtual void Tick(float DeltaTime) override;

	virtual void Interact_Implementation(APawn* InstigatorPawn);

	virtual bool CanInteract_Implementation(APawn* InstigatorPawn);

	virtual bool IsEnabled_Implementation();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnRep_bIsSwitched();

	UPROPERTY(ReplicatedUsing = "OnRep_bIsSwitched", BlueprintReadOnly, Category = "Rogue|Lever")
	bool bIsSwitched = false;

	UPROPERTY(EditAnywhere)
	float SwitchedLeverPitch = -30.f;

	UPROPERTY(EditAnywhere)
	float NotSwitchedLeverPitch = 30.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Rogue|Lever|Quest")
	FGameplayTag ObjectiveTag;
};
