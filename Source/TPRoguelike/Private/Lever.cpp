// Made by Jan Puto 2023 :D


#include "Lever.h"
#include "Kismet/GameplayStatics.h"
#include "GameState/SGameState.h"
#include "Components/SQuestManagerComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Net/UnrealNetwork.h"

// Sets default values
ALever::ALever()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BaseMesh"));
	RootComponent = BaseMesh;

	OnSwitchParticleComp = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("OnSwitchParticleComp"));
	OnSwitchParticleComp->SetupAttachment(RootComponent);
	OnSwitchParticleComp->SetAutoActivate(false);

	LeverMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LidMesh"));
	LeverMesh->SetupAttachment(BaseMesh);
	LeverMesh->SetRelativeRotation(FRotator(NotSwitchedLeverPitch, 0.0f, 0.0f));

	bReplicates = true;
}

void ALever::BeginPlay()
{
	Super::BeginPlay();
	
}

void ALever::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ALever::Interact_Implementation(APawn* InstigatorPawn)
{
	if (!bIsSwitched)
	{
		bIsSwitched = true;
		SwitchLever();

		if (ObjectiveTag.IsValid())
		{
			TObjectPtr<ASGameState> GameState = Cast<ASGameState>(UGameplayStatics::GetGameState(GetWorld()));
			if (GameState)
			{
				TObjectPtr<USQuestManagerComponent> QuestManager = GameState->GetQuestManager();
				if (QuestManager)
				{
					QuestManager->ServerOnlyAddObjectiveStat(ObjectiveTag, 1);
				}
			}
		}
	}
}

bool ALever::CanInteract_Implementation(APawn* InstigatorPawn)
{
	bool bCanInteract = false;
	if (!bIsSwitched)
	{
		if (ObjectiveTag.IsValid())
		{
			TObjectPtr<ASGameState> GameState = Cast<ASGameState>(UGameplayStatics::GetGameState(GetWorld()));
			if (GameState)
			{
				TObjectPtr<USQuestManagerComponent> QuestManager = GameState->GetQuestManager();
				if (QuestManager)
				{
					bCanInteract = QuestManager->IsObjectiveActive(ObjectiveTag);

					if (!bCanInteract)
					{
						FString DebugMsg = "Objective: " + ObjectiveTag.ToString() + " is not active. You cannot interact with the lever right now.";
						GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, DebugMsg);
					}
				}
			}
		}
		else
		{
			// If we dont have ObjectiveTag assigned we can always interact
			bCanInteract = true;

			FString DebugMsg = "No ObjectiveTag assigned. You can interact.";
			GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, DebugMsg);
		}
	}

	return bCanInteract;
}

bool ALever::IsEnabled_Implementation()
{
	return true;
}

void ALever::SwitchLever()
{
	LeverMesh->SetRelativeRotation(FRotator(SwitchedLeverPitch, 0.0f, 0.0f));
	OnSwitchParticleComp->Activate();
}

void ALever::OnRep_bIsSwitched()
{
	if (bIsSwitched)
	{
		SwitchLever();
	}
}

void ALever::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ALever, bIsSwitched);
}