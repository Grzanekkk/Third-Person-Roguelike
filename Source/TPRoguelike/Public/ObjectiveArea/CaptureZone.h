// Made by Jan Puto 2022 :D

#pragma once

#include "CoreMinimal.h"
#include "ObjectiveArea/ObjectiveArea.h"
#include "GameplayTagContainer.h"
#include "CaptureZone.generated.h"

class UCapsuleComponent;
class ASCharacter;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnZoneCapturedEvent, ACaptureZone*, CapturedZone, TArray<ASCharacter*>, PlayersResponsibleForCapture);

/**
 * 
 */
UCLASS()
class TPROGUELIKE_API ACaptureZone : public AObjectiveArea
{
	GENERATED_BODY()
	
public:
	ACaptureZone();

	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Rogue|Components")
	TObjectPtr<UCapsuleComponent> TriggerBox = nullptr;

	UPROPERTY(VisibleAnywhere, Category = "Rogue|Components")
	TObjectPtr<UStaticMeshComponent> AreaIndicatorMesh = nullptr;

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

	virtual void PostInitializeComponents() override;

	virtual void BeginPlay() override;

	UFUNCTION()
	void CalculateCapturePoints();

	UFUNCTION()
	virtual void StartOverlapingZone(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	virtual void StopOverlapingZone(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

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

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Rogue|CaptureZone")
	TArray<TObjectPtr<ASCharacter>> PlayersInsideZone;

	UPROPERTY(EditAnywhere, Category = "Rogue|CaptureZone|Quest")
	FGameplayTag ObjectiveTag;
};
