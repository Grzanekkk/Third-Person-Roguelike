// Made by Jan Puto 2023 :D

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameplayTagContainer.h"
#include "SStartQuestZone.generated.h"

class USQuestBase;
class UBoxComponent;

UCLASS()
class TPROGUELIKE_API ASStartQuestZone : public AActor
{
	GENERATED_BODY()
	
public:	
	ASStartQuestZone();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Rogue|Component")
	TObjectPtr<UBoxComponent> OverlapBox = nullptr;

protected:
	virtual void BeginPlay() override;

	void PostInitializeComponents() override;

	UFUNCTION()
	void OnBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Rogue|QuestZone")
	FGameplayTagContainer ObjectiveTag;
};
