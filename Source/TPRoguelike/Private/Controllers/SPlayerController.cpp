// Made by Jan Puto 2022 :D


#include "Controllers/SPlayerController.h"
#include "FunctionLibrary/GameplayFunctionLibrary.h"
#include "Components/SQuestManagerComponent.h"
#include "UI/QuestWidget.h"

void ASPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (IsLocalController())
	{
		TObjectPtr<USQuestManagerComponent> QuestManager = UGameplayFunctionLibrary::GetQuestManager(GetWorld());
		if (QuestManager)
		{
			QuestManager->OnQuestSystemActivate.AddDynamic(this, &ASPlayerController::OnQuestSystemActivate);
		}
	}
}

void ASPlayerController::SetPawn(APawn* InPawn)
{
	Super::SetPawn(InPawn);

	OnPawnChanged.Broadcast(InPawn);
}

void ASPlayerController::OnQuestSystemActivate()
{
	if (ensure(QuestWidgetClass))
	{
		QuestWidgetInstance = CreateWidget<UQuestWidget>(GetWorld(), QuestWidgetClass);
		if (QuestWidgetInstance)
		{
			QuestWidgetInstance->AddToViewport();
			QuestWidgetInstance->OnQuestSystemActivate();
		}
	}
}
