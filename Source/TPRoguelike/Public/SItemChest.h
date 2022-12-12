// Made by Jan Puto 2022 :D

#pragma once

#include "CoreMinimal.h"
#include "SGameplayInterface.h"
#include "GameFramework/Actor.h"
#include "SItemChest.generated.h"

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

protected:

	UPROPERTY(ReplicatedUsing="OnRep_bIsOpened", BlueprintReadOnly, Category = "Rogue|State")
	bool bIsOpened = false;

	UFUNCTION()
	void OnRep_bIsOpened();

	UFUNCTION(BlueprintImplementableEvent, Category = "Rogue|State")
	void OpenLid();

	UPROPERTY(VisibleAnywhere, Category = "Rogue|Components")
	UStaticMeshComponent* BaseMesh;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Rogue|Components")
	UStaticMeshComponent* LidMesh;
	
	UPROPERTY(VisibleAnywhere, Category = "Rogue|Components")
	class UParticleSystemComponent* ParticleComp;
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
