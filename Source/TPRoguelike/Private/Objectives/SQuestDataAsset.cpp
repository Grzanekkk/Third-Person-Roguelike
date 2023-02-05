// Made by Jan Puto 2023 :D


#include "Objectives/SQuestDataAsset.h"

int32 USQuestDataAsset::GetGoalValueOfObjective(FGameplayTag ObjectiveTag)
{
	for (FObjectiveDefaults ObjectiveDefaults : ObjectivesGoal)
	{
		if (ObjectiveDefaults.ObjectiveTag == ObjectiveTag)
		{
			return ObjectiveDefaults.GoalValue;
		}
	}

	return -1;
}

bool USQuestDataAsset::IsObjectiveFinished(FGameplayTag ObjectiveTag, int32 CurrentValue)
{
	for (FObjectiveDefaults ObjectiveDefaults : ObjectivesGoal)
	{
		if (ObjectiveDefaults.ObjectiveTag == ObjectiveTag)
		{
			return CurrentValue >= ObjectiveDefaults.GoalValue;
		}
	}

	return false;
}
