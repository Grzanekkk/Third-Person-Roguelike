// Made by Jan Puto 2023 :D

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SZone_AllPlayersInside.generated.h"

UCLASS()
class TPROGUELIKE_API ASZone_AllPlayersInside : public AActor
{
	GENERATED_BODY()
	
public:	
	ASZone_AllPlayersInside();

	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;



};
