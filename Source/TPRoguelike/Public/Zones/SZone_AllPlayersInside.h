// Made by Jan Puto 2023 :D

#pragma once

#include "CoreMinimal.h"
#include "Zones/SZone_Box.h"
#include "GameplayTagContainer.h"
#include "SZone_AllPlayersInside.generated.h"

UCLASS()
class TPROGUELIKE_API ASZone_AllPlayersInside : public ASZone_Box
{
	GENERATED_BODY()

protected:
	virtual void PostInitializeComponents() override;

	UFUNCTION()
	void CheckIfAllPlayersAreInside(int32 PlayersInside);

	UPROPERTY(EditAnywhere, Category = "Rogue|Zone")
	TObjectPtr<AActor> ActorToInteractOnCapture;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Rogue|Zone|Quest")
	FGameplayTag ObjectiveTag;
};
