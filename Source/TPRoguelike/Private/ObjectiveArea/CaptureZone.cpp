// Made by Jan Puto 2022 :D


#include "ObjectiveArea/CaptureZone.h"
#include "Components/CapsuleComponent.h"

ACaptureZone::ACaptureZone()
{
	TriggerBox = CreateDefaultSubobject<UCapsuleComponent>("TriggerBox");
	RootComponent = TriggerBox;
}

void ACaptureZone::BeginPlay()
{
	Super::BeginPlay();


}