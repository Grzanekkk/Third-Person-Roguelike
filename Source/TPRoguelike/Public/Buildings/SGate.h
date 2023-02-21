// Made by Jan Puto 2023 :D

#pragma once

#include "CoreMinimal.h"
#include "SGameplayInterface.h"
#include "GameFramework/Actor.h"
#include "SGate.generated.h"

class USphereComponent;

UCLASS()
class TPROGUELIKE_API ASGate : public AActor, public ISGameplayInterface
{
	GENERATED_BODY()
	
public:	
	ASGate();

	virtual void Tick(float DeltaTime) override;

	virtual void Interact_Implementation(APawn* InstigatorPawn);

	virtual bool CanInteract_Implementation(APawn* InstigatorPawn);

	virtual bool IsEnabled_Implementation();

	UFUNCTION()
	void OpenGate();

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void OpeningGateTick();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Rogue|Components")
	TObjectPtr<USphereComponent> RootSphere;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Rogue|Components")
	TObjectPtr<USphereComponent> LeftWingPivot;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Rogue|Components")
	TObjectPtr<USphereComponent> RightWingPivot;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Rogue|Components")
	TObjectPtr<UStaticMeshComponent> LeftWing;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Rogue|Components")
	TObjectPtr<UStaticMeshComponent> RightWing;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Rogue|Gate|Settings")
	float GateOpeningSpeed = 50.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Rogue|Gate|Settings", meta = (ClampMin = -1, ClampMax = 1))
	int32 GateOpeningDirection = 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Rogue|Gate|Settings")
	float OpenedGateAngle = 150.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Rogue|Gate")
	float CurrentGateAngle = 0.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Rogue|Gate")
	bool bIsGateOpened;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Rogue|Gate", Replicated)
	bool bIsGateOpening;
};
