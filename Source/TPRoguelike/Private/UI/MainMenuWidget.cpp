// Made by Jan Puto 2023 :D


#include "UI/MainMenuWidget.h"
#include "Components/VerticalBox.h"


void UMainMenuWidget::NativeConstruct()
{
	EOSSubsystem = GetGameInstance()->GetSubsystem<UEOSSubsystem>();
	if (ensure(EOSSubsystem))
	{
		EOSSubsystem->OnFindAllSessionStarted.AddDynamic(this, &UMainMenuWidget::OnFindAllSessionsStarted);
		EOSSubsystem->OnFindAllSessionFinished.AddDynamic(this, &UMainMenuWidget::OnFindAllSessionsFinished);
	}

	VB_SearchResult->SetVisibility(ESlateVisibility::Collapsed);
}

void UMainMenuWidget::OnFindAllSessionsStarted()
{
	VB_SearchResult->SetVisibility(ESlateVisibility::Visible);
}

void UMainMenuWidget::OnFindAllSessionsFinished(bool bWasSuccessful, const FOnlineSessionSearch_Rogue& SearchResults)
{
}


