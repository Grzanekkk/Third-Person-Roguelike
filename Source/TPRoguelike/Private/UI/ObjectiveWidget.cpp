// Made by Jan Puto 2023 :D


#include "UI/ObjectiveWidget.h"
#include "FunctionLibrary/LogsFunctionLibrary.h"
#include "FunctionLibrary/GameplayFunctionLibrary.h"
#include "Components/SQuestManagerComponent.h"
#include "Enums/SEnums_Logs.h"

UObjectiveWidget::UObjectiveWidget()
{
}

void UObjectiveWidget::Initialize(FGameplayTag ObjectiveAttached)
{
	if (ObjectiveAttached.IsValid())
	{
		bFoundMatchingObjectiveGoals = false;

		TObjectPtr<USQuestManagerComponent> QuestManager = UGameplayFunctionLibrary::GetQuestManager(GetWorld());
		if (QuestManager)
		{
			//QuestManager->
		}
	}

	FString Msg = ("Filed to Initialize ObjectiveWidget!");
	ULogsFunctionLibrary::LogOnScreen(GetWorld(), Msg, ERogueLogCategory::ERROR);
}
