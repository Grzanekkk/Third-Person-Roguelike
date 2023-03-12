// Made by Jan Puto 2023 :D


#include "UI/ObjectiveWidget.h"
#include "FunctionLibrary/LogsFunctionLibrary.h"
#include "FunctionLibrary/GameplayFunctionLibrary.h"
#include "Components/SQuestManagerComponent.h"
#include "Components/TextBlock.h"
#include "Components/CheckBox.h"
#include "Components/HorizontalBox.h"
#include "Styling/SlateColor.h"
#include "Enums/SEnums_Objectives.h"
#include "Objectives/SQuestDataAsset.h"

void UObjectiveWidget::Init(const FGameplayTag& _ObjectiveAttached)
{
	if (_ObjectiveAttached.IsValid())
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
	else
	{
		FString Msg = ("Filed to Initialize ObjectiveWidget!");
		ULogsFunctionLibrary::LogOnScreen(GetWorld(), Msg, ERogueLogCategory::ERROR);
	}
}

void UObjectiveWidget::OnObjectiveStateChanged(const FGameplayTag& ObjectiveTag, EObjectiveState ObjectiveState)
{
	if (ObjectiveTag == ObjectiveAttached)
	{
		switch (ObjectiveState)
		{
			case EObjectiveState::IN_PROGRESS:
			{
				TObjectPtr<USQuestManagerComponent> QuestManager = UGameplayFunctionLibrary::GetQuestManager(GetWorld());
				if (QuestManager)
				{
					for (const FObjectiveDefaults& ObjectiveDefault : QuestManager->GetObjectiveGoals()->ObjectivesGoal)
					{
						if (ObjectiveDefault.ObjectiveTag == ObjectiveAttached)
						{
							bFoundMatchingObjectiveGoals = true;
							ObjectiveNameTEXT->SetText(ObjectiveDefault.DisplayName);
							ObjectiveGoalValue = ObjectiveDefault.GoalValue;

							bIsStatObjective = ObjectiveGoalValue > 1;
							if (bIsStatObjective)
							{
								TriggerObjectiveBox->SetVisibility(ESlateVisibility::Collapsed);
								StatObjectiveBox->SetVisibility(ESlateVisibility::Visible);
							}
							else
							{
								TriggerObjectiveBox->SetVisibility(ESlateVisibility::Visible);
								StatObjectiveBox->SetVisibility(ESlateVisibility::Collapsed);
							}

							break;
						}
					}

					if (!bFoundMatchingObjectiveGoals)
					{
						ObjectiveNameTEXT->SetText(FText::FromString("Please fill objective info in DA_Objectives"));
					}
				}

				break;
			}
			case EObjectiveState::FINISHED:
			{
				SetObjectiveObjectiveNameColor(CompletedObjectiveColor);
				if (!bIsStatObjective)
				{
					ObjectiveFinishedCheckBox->SetCheckedState(ECheckBoxState::Checked);
				}

				break;
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
		FText Text = FText::FromString(FString::Printf(TEXT("%i/%i"), CurrentValue, ObjectiveGoalValue));
		StatObjectiveTEXT->SetText(Text);
	}
}
