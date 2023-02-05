// Made by Jan Puto 2022 :D


#include "Components/SQuestManagerComponent.h"


USQuestManagerComponent::USQuestManagerComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void USQuestManagerComponent::BeginPlay()
{
	Super::BeginPlay();
}

void USQuestManagerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void USQuestManagerComponent::AddObjectiveStat(FGameplayTag ObjectiveTag, int32 Stat)
{

}