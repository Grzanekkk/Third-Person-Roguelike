// Made by Jan Puto 2023 :D


#include "Objectives/SObjectiveSequenceDataAsset.h"
#include "FunctionLibrary/LogsFunctionLibrary.h"

FGameplayTagContainer& USObjectiveSequenceDataAsset::GetFirstObjectives()
{
	return ObjectivesOrder[0];
}

FGameplayTagContainer& USObjectiveSequenceDataAsset::GetNextObjectives(FGameplayTag CurrentObjective)
{
	// We are providing one of the Objectives in current step
	
	// Looking through every step of the sequance
	for (int32 ContainerIndex = 0; ContainerIndex < ObjectivesOrder.Num(); ContainerIndex++)
	{
		// Looping through every objective in this sequance
		for(int32 TagIndex = 0; TagIndex < ObjectivesOrder[ContainerIndex].Num(); TagIndex++)
		{
			// Checking if objectives match
			if (ObjectivesOrder[ContainerIndex].GetByIndex(TagIndex) == CurrentObjective)
			{
				if (ObjectivesOrder.Num() > ContainerIndex + 1)
				{
					return ObjectivesOrder[ContainerIndex + 1];
				}
			}
		}
	}

	FString DebugMsg = "NO NEXT OBJECTIVE FOUND IN: USObjectiveSequenceDataAsset! Returning LAST objecitve in sequance!";
	ULogsFunctionLibrary::LogOnScreen(GetWorld(), DebugMsg, ERogueLogCategory::ERROR);

	return ObjectivesOrder.Last();
}

bool USObjectiveSequenceDataAsset::IsNextStepAvalible(FGameplayTag CurrentObjective)
{
	// Looking through every step of the sequance
	for (int32 ContainerIndex = 0; ContainerIndex < ObjectivesOrder.Num(); ContainerIndex++)
	{
		// Looping through every objective in this sequance
		for (int32 TagIndex = 0; TagIndex < ObjectivesOrder[ContainerIndex].Num(); TagIndex++)
		{
			// Checking if objectives match
			if (ObjectivesOrder[ContainerIndex].GetByIndex(TagIndex) == CurrentObjective)
			{
				return ObjectivesOrder.Num() > ContainerIndex + 1;
			}
		}
	}

	return false;
}
