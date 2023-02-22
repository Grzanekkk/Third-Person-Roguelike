// Made by Jan Puto 2023 :D


#include "Zones/SZone_Capsule.h"
#include "Components/CapsuleComponent.h"

ASZone_Capsule::ASZone_Capsule()
{
	TriggerCapsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("TriggerCapsule"));
	TriggerCapsule->SetupAttachment(RootComponent);
	TriggerAreaComponent = TriggerCapsule;
}