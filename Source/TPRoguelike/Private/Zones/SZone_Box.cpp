// Made by Jan Puto 2023 :D


#include "Zones/SZone_Box.h"
#include "Components/BoxComponent.h"

ASZone_Box::ASZone_Box()
{
	TriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));
	TriggerBox->SetupAttachment(RootComponent);
	TriggerAreaComponent = TriggerBox;
}