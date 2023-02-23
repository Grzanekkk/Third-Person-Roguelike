// Made by Jan Puto 2023 :D

#pragma once

#include "CoreMinimal.h"
#include "Zones/SZone_Box.h"
#include "SZone_AllPlayersInside.generated.h"

UCLASS()
class TPROGUELIKE_API ASZone_AllPlayersInside : public ASZone_Box
{
	GENERATED_BODY()
	
public:	
	ASZone_AllPlayersInside();

protected:
	virtual void PostInitializeComponents() override;

	UFUNCTION()
	void CheckIfAllPlayersAreInside(int32 PlayersInside);

	UPROPERTY(EditAnywhere, Category = "Rogue|Zone")
	TObjectPtr<AActor> ActorToInteractOnSwitch;
};
