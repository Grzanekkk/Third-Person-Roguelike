// Made by Jan Puto 2023 :D


#include "Lever.h"
#include "Kismet/GameplayStatics.h"
#include "GameState/SGameState.h"
#include "Components/SQuestManagerComponent.h"
#include "Net/UnrealNetwork.h"

// Sets default values
ALever::ALever()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ALever::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ALever::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ALever::Interact_Implementation(APawn* InstigatorPawn)
{

}

bool ALever::CanInteract_Implementation(APawn* InstigatorPawn)
{
	TObjectPtr<ASGameState> GameState = Cast<ASGameState>(UGameplayStatics::GetGameState(GetWorld()));
	if (GameState)
	{
		TObjectPtr<USQuestManagerComponent> QuestManager = GameState->GetQuestManager();
		if (QuestManager)
		{
			return QuestManager->IsObjectiveActive(ObjectiveTag);
		}
	}

	return false;
}

bool ALever::IsEnabled_Implementation()
{
	return true;
}

void ALever::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ALever, bIsSwitched);
}