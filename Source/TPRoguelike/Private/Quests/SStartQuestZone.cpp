// Made by Jan Puto 2023 :D


#include "Quests/SStartQuestZone.h"
#include "Components/BoxComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "GameState/SGameState.h"
#include "Components/SQuestManagerComponent.h"
#include "Characters/SCharacter.h"


ASStartQuestZone::ASStartQuestZone()
{
	PrimaryActorTick.bCanEverTick = true;

	OverlapBox = CreateDefaultSubobject<UBoxComponent>(TEXT("OverlapBox"));
	RootComponent = OverlapBox;
}

void ASStartQuestZone::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	OverlapBox->OnComponentBeginOverlap.AddDynamic(this, &ASStartQuestZone::OnBoxOverlap);
}

void ASStartQuestZone::OnBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (UKismetSystemLibrary::IsServer(GetWorld()))
	{
		TObjectPtr<ASCharacter> ACharacter = Cast<ASCharacter>(OtherActor);
		if (ACharacter)
		{
			TObjectPtr<ASGameState> GameState = Cast<ASGameState>(UGameplayStatics::GetGameState(GetWorld()));
			if (GameState)
			{
				TObjectPtr<USQuestManagerComponent> QuestManager = GameState->GetQuestManager();
				if (QuestManager)
				{
					QuestManager->ServerOnlyStartObjective(ObjectiveTag.GetByIndex(0));
				}
			}
		}
	}
}