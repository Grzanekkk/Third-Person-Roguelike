// Made by Jan Puto 2023 :D


#include "Zones/SZone_AllPlayersInside.h"
#include "Kismet/GameplayStatics.h"
#include "SGameplayInterface.h"
#include "GameState/SGameState.h"

void ASZone_AllPlayersInside::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	OnNumberOfPlayersInsideZoneChanged.AddDynamic(this, &ASZone_AllPlayersInside::CheckIfAllPlayersAreInside);
}

void ASZone_AllPlayersInside::CheckIfAllPlayersAreInside(int32 PlayersInside)
{
	TObjectPtr<ASGameState> SGameState = Cast<ASGameState>(UGameplayStatics::GetGameState(GetWorld()));
	if (SGameState)
	{
		if (PlayersInside >= SGameState->GetNumberOfConnectedPlayers())
		{
			if (UKismetSystemLibrary::IsServer(GetWorld()))
			{
				if (ActorToInteractOnSwitch && ActorToInteractOnSwitch->Implements<USGameplayInterface>() && ISGameplayInterface::Execute_IsEnabled(ActorToInteractOnSwitch))
				{
					if (ISGameplayInterface::Execute_CanInteract(ActorToInteractOnSwitch, nullptr))
					{
						ISGameplayInterface::Execute_Interact(ActorToInteractOnSwitch, nullptr);
					}
				}

				//// For Quests
				//if (UKismetSystemLibrary::IsServer(GetWorld()))
				//{
				//	TObjectPtr<ASGameState> GameState = Cast<ASGameState>(UGameplayStatics::GetGameState(GetWorld()));
				//	if (GameState)
				//	{
				//		TObjectPtr<USQuestManagerComponent> QuestManager = GameState->GetQuestManager();
				//		if (QuestManager)
				//		{
				//			QuestManager->ServerOnlyAddObjectiveStat(ObjectiveTag, 1);
				//		}
				//	}
				//}
			}


			// we might do some visuals here
		}
	}
}
