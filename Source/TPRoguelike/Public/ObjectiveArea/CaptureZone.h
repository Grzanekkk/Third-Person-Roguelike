// Made by Jan Puto 2022 :D

#pragma once

#include "CoreMinimal.h"
#include "ObjectiveArea/ObjectiveArea.h"
#include "GameplayTagContainer.h"
#include "Zones/SZone_Capsule.h"
#include "CaptureZone.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnZoneCapturedEvent, ACaptureZone*, CapturedZone, TArray<ASCharacter*>, PlayersResponsibleForCapture);

/**
 * 
 */
UCLASS()
class TPROGUELIKE_API ACaptureZone : public ASZone_Capsule
{
	GENERATED_BODY()
	
public:
	ACaptureZone();

	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere, Category = "Rogue|Components")
	TObjectPtr<UStaticMeshComponent> FlagPoleMesh = nullptr;

	UPROPERTY(VisibleAnywhere, Category = "Rogue|Components")
	TObjectPtr<UStaticMeshComponent> FlagMesh = nullptr;

	UPROPERTY(VisibleAnywhere, Category = "Rogue|Components")
	TObjectPtr<UStaticMeshComponent> QuestIndicatorMesh = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Rogue|Flag")
	float FlagTargetHight = 560.f;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Rogue|CaptureZone")
	bool IsBeingCaptured();

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Rogue|CaptureZone")
	float GetZoneCapturePercent();

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Rogue|CaptureZone")
	float GetCaptureSpeedMultiplier();

	UPROPERTY()
	FOnZoneCapturedEvent OnZoneCapturedEvent;

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void CalculateCapturePoints();

	UFUNCTION()
	void OnZoneCaptured();

	UFUNCTION()
	void OnRep_CurrentCapPoints();

	UFUNCTION()
	void SetFlagHight();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Rogue|CaptureZone")
	float MaxCapPoints = 1000.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Rogue|CaptureZone", ReplicatedUsing="OnRep_CurrentCapPoints")
	float CurrentCapPoints = 0.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Rogue|CaptureZone")
	float CapPointPerTick = 20.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Rogue|CaptureZone")
	TArray<float> CapPointPerPlayerMultiplier /*  [1.0f, 1.5f] */;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Rogue|CaptureZone")
	bool bIsCaptured = false;

	UPROPERTY(EditAnywhere, Category = "Rogue|CaptureZone|Quest")
	FGameplayTag ObjectiveTag;

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};
