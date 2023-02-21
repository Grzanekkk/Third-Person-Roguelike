// Made by Jan Puto 2023 :D


#include "Zones/SZone_AllPlayersInside.h"

// Sets default values
ASZone_AllPlayersInside::ASZone_AllPlayersInside()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ASZone_AllPlayersInside::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASZone_AllPlayersInside::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

