// Made by Jan Puto 2023 :D


#include "Subsystems/SWorldSubsystem.h"
#include "Components/SQuestManagerComponent.h"
#include "FunctionLibrary/GameplayFunctionLibrary.h"

void USWorldSubsystem::SetObjectiveDataForLevelAndStart(USObjectiveSequenceDataAsset* StartingObjectiveSequance, USQuestDataAsset* NewObjectivesGoals)
{
	TObjectPtr<USQuestManagerComponent> QuestManager = UGameplayFunctionLibrary::GetQuestManager(GetWorld());
	if (QuestManager)
	{
		QuestManager->ServerOnlySetObjectiveDataAndStart(StartingObjectiveSequance, NewObjectivesGoals);
	}
}
