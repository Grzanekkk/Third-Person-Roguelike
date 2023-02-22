// Made by Jan Puto 2022 :D

#pragma once

#include "CoreMinimal.h"
#include "SGameplayInterface.h"
#include "GameFramework/Actor.h"
#include "SItemChest.generated.h"

class UParticleSystemComponent;

UCLASS()
class TPROGUELIKE_API ASItemChest : public AActor, public ISGameplayInterface
{
	GENERATED_BODY()
	
public:

	UPROPERTY(EditAnywhere)
	float TargetLidPitch;
	
	ASItemChest();
	
	virtual void Interact_Implementation(APawn* InstigatorPawn);

	virtual bool CanInteract_Implementation(APawn* InstigatorPawn);

	virtual bool IsEnabled_Implementation();

	virtual void OnActorLoaded_Implementation();

	virtual bool CanBeInteractedByPlayerCharacter_Implementation();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnRep_bIsOpened();

	UFUNCTION(BlueprintImplementableEvent, Category = "Rogue|State")
	void OpenCloseLid(bool bOpen);

	UPROPERTY(SaveGame, ReplicatedUsing = "OnRep_bIsOpened", BlueprintReadOnly, Category = "Rogue|State")
	bool bIsOpened = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Rogue|State")
	float OpenLidRotation = 110.f;

	UPROPERTY(VisibleAnywhere, Category = "Rogue|Components")
	TObjectPtr<UStaticMeshComponent> BaseMesh;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Rogue|Components")
	TObjectPtr<UStaticMeshComponent> LidMesh;
	
	UPROPERTY(VisibleAnywhere, Category = "Rogue|Components")
	TObjectPtr<UParticleSystemComponent> ParticleComp;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
