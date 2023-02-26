// Made by Jan Puto 2023 :D


#include "Lever.h"
#include "Kismet/GameplayStatics.h"
#include "GameState/SGameState.h"
#include "Components/SQuestManagerComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Enums/SEnums_Logs.h"
#include "SGameplayInterface.h"
#include "FunctionLibrary/LogsFunctionLibrary.h"
#include "Net/UnrealNetwork.h"
#include "FunctionLibrary/GameplayFunctionLibrary.h"

ALever::ALever()
{	
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

void ALever::Interact_Implementation(APawn* InstigatorPawn)
{
	if (!bIsSwitched)
	{
		bIsSwitched = true;
		SwitchLever();

		UGameplayFunctionLibrary::AddObjectiveStat(GetWorld(), ObjectiveTag, 1);

		UGameplayFunctionLibrary::IntaractIfPossible(GetWorld(), ActorToInteractOnSwitch, nullptr);
	}
}

bool ALever::CanInteract_Implementation(APawn* InstigatorPawn)
{
	bool bCanInteract = false;
	if (!bIsSwitched)
	{
		if (ObjectiveTag.IsValid())
		{
			TObjectPtr<USQuestManagerComponent> QuestManager = UGameplayFunctionLibrary::GetQuestManager(GetWorld());
			if (QuestManager)
			{
				bCanInteract = QuestManager->IsObjectiveActive(ObjectiveTag);

				if (!bCanInteract)
				{
					FString DebugMsg = "Objective: " + ObjectiveTag.ToString() + " is not active. You cannot interact with the lever right now.";
					ULogsFunctionLibrary::LogOnScreen(GetWorld(), DebugMsg, ERogueLogCategory::WARNING);
				}
			}
		}
		else
		{
			// If we dont have ObjectiveTag assigned we can always interact
			bCanInteract = true;

			FString DebugMsg = "No ObjectiveTag assigned. You can interact.";
			ULogsFunctionLibrary::LogOnScreen(GetWorld(), DebugMsg, ERogueLogCategory::LOG);
		}
	}

	return bCanInteract;
}

bool ALever::IsEnabled_Implementation()
{
	return true;
}

bool ALever::CanBeInteractedByPlayerCharacter_Implementation()
{
	return true;
}

void ALever::SwitchLever()
{
	// Visual Only

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