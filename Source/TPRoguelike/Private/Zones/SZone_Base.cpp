// Made by Jan Puto 2023 :D


#include "Zones/SZone_Base.h"
#include "Enums/SEnums_Zones.h"
#include "Components/ShapeComponent.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Characters/SCharacter.h"

ASZone_Base::ASZone_Base()
{
	PrimaryActorTick.bCanEverTick = true;

	RootSphere = CreateDefaultSubobject<USphereComponent>(TEXT("RootSphere"));
	SetRootComponent(RootSphere);

	AreaIndicatorMesh = CreateDefaultSubobject<UStaticMeshComponent>("AreaIndicatorMesh");
	AreaIndicatorMesh->SetupAttachment(RootComponent);
}

void ASZone_Base::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	TriggerAreaComponent->OnComponentBeginOverlap.AddDynamic(this, &ASZone_Base::StartOverlapingZone);
	TriggerAreaComponent->OnComponentEndOverlap.AddDynamic(this, &ASZone_Base::StopOverlapingZone);
}

void ASZone_Base::StartOverlapingZone(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	TObjectPtr<ASCharacter> PlayerCharacter = Cast<ASCharacter>(OtherActor);
	if (PlayerCharacter && !PlayersInsideZone.Contains(PlayerCharacter))
	{
		// We know new player entered the zone
		PlayersInsideZone.Add(PlayerCharacter);
		OnNumberOfPlayersInsideZoneChanged.Broadcast(PlayersInsideZone.Num());
	}
}

void ASZone_Base::StopOverlapingZone(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	TObjectPtr<ASCharacter> PlayerCharacter = Cast<ASCharacter>(OtherActor);
	if (PlayerCharacter && PlayersInsideZone.Contains(PlayerCharacter))
	{
		// We know player left the zone
		PlayersInsideZone.Remove(PlayerCharacter);
		OnNumberOfPlayersInsideZoneChanged.Broadcast(PlayersInsideZone.Num());
	}
}