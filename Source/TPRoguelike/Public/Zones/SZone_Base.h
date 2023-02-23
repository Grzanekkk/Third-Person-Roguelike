// Made by Jan Puto 2023 :D

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SZone_Base.generated.h"

class ASCharacter;
class UCapsuleComponent;
class UBoxComponent;
class UShapeComponent;
class USphereComponent;

enum class EZoneShape : uint8;


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnNumberOfPlayersInsideZoneChanged, int32, PlayersInsideZone);


UCLASS()
class TPROGUELIKE_API ASZone_Base : public AActor
{
	GENERATED_BODY()
	
public:	
	ASZone_Base();

	UPROPERTY()
	FOnNumberOfPlayersInsideZoneChanged OnNumberOfPlayersInsideZoneChanged;

protected:
	virtual void PostInitializeComponents() override;

	UFUNCTION()
	virtual void StartOverlapingZone(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	virtual void StopOverlapingZone(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Rogue|Components")
	TObjectPtr<USphereComponent> RootSphere;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Rogue|Components")
	TObjectPtr<UShapeComponent> TriggerAreaComponent = nullptr;

	UPROPERTY(VisibleAnywhere, Category = "Rogue|Components")
	TObjectPtr<UStaticMeshComponent> AreaIndicatorMesh = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Rogue|Zone")
	TArray<TObjectPtr<ASCharacter>> PlayersInsideZone;
};