// Made by Jan Puto 2023 :D


#include "UI/ObjectiveWidget.h"
#include "FunctionLibrary/LogsFunctionLibrary.h"
#include "FunctionLibrary/GameplayFunctionLibrary.h"
#include "Components/SQuestManagerComponent.h"
#include "Components/TextBlock.h"
#include "Components/CheckBox.h"
#include "Enums/SEnums_Logs.h"
#include "Enums/SEnums_Objectives.h"

void UObjectiveWidget::Initialize(const FGameplayTag& _ObjectiveAttached)
{
	if (ObjectiveAttached.IsValid())
	{
		ObjectiveAttached = _ObjectiveAttached;
		bFoundMatchingObjectiveGoals = false;

		TObjectPtr<USQuestManagerComponent> QuestManager = UGameplayFunctionLibrary::GetQuestManager(GetWorld());
		if (QuestManager)
		{
			QuestManager->OnObjectiveStateChangedEvent.AddDynamic(this, &UObjectiveWidget::OnObjectiveStateChanged);
			OnObjectiveStateChanged(ObjectiveAttached, QuestManager->GetActiveObjectiveState(ObjectiveAttached));

			if (bIsStatObjective)
			{
				QuestManager->OnObjectiveValueChangedEvent.AddDynamic(this, &UObjectiveWidget::OnObjectiveValueChanged);
				OnObjectiveValueChanged(ObjectiveAttached, QuestManager->GetActiveObjectiveValue(ObjectiveAttached), 0);
			}
		}
	}

	FString Msg = ("Filed to Initialize ObjectiveWidget!");
	ULogsFunctionLibrary::LogOnScreen(GetWorld(), Msg, ERogueLogCategory::ERROR);
}

void UObjectiveWidget::OnObjectiveStateChanged(const FGameplayTag& ObjectiveTag, EObjectiveState ObjectiveState)
{
	if (ObjectiveTag == ObjectiveAttached)
	{
		switch (ObjectiveState)
		{
			case EObjectiveState::IN_PROGRESS:
			{

			}
			case EObjectiveState::FINISHED:
			{
				ObjectiveNameTEXT->SetColorAndOpacity(CompletedObjectiveColor);
				if (!bIsStatObjective)
				{
					ObjectiveFinishedCheckBox->SetCheckedState(ECheckBoxState::Checked);
				}
			}
			default:
				break;
		}
	}
}

void UObjectiveWidget::OnObjectiveValueChanged(const FGameplayTag& ObjectiveTag, int32 CurrentValue, int32 DeltaValue)
{
	if (ObjectiveTag == ObjectiveAttached)
	{
		FText Text = FText::FromString(FString::Printf(TEXT("%i/%i"), CurrentValue, StatObjectiveGoalValue));
		StatObjectiveTEXT->SetText(Text);
	}
}
