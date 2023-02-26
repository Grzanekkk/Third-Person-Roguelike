// Made by Jan Puto 2023 :D


#include "UI/QuestWidget.h"
#include "UI/ObjectiveWidget.h"
#include "FunctionLibrary/GameplayFunctionLibrary.h"
#include "Components/SQuestManagerComponent.h"
#include "Components/VerticalBox.h"
#include "Enums/SEnums_Objectives.h"

void UQuestWidget::OnQuestSystemActivate()
{
	TObjectPtr<USQuestManagerComponent> QuestManager = UGameplayFunctionLibrary::GetQuestManager(GetWorld());
	if (QuestManager)
	{
		QuestManager->OnObjectiveStateChangedEvent.AddDynamic(this, &UQuestWidget::OnObjectiveStateChanged);
	}
}

void UQuestWidget::OnObjectiveStateChanged(FGameplayTag ObjectiveTag, EObjectiveState ObjectiveState)
{
	if (ensure(ObjectiveWidgetClass))
	{
		TObjectPtr<UObjectiveWidget> ObjectiveWidgetInstance = CreateWidget<UObjectiveWidget>(GetWorld(), ObjectiveWidgetClass);
		if (ObjectiveWidgetInstance)
		{
			ObjectiveWidgetInstance->Initialize(ObjectiveTag);
			ObjectiveBox->AddChild(ObjectiveWidgetInstance);
			ObjectiveWidgets.Add(ObjectiveWidgetInstance);
		}
	}
}
