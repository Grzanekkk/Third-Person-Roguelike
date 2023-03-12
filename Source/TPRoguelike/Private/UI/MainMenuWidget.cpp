// Made by Jan Puto 2023 :D


#include "UI/MainMenuWidget.h"
#include "Components/VerticalBox.h"
#include "UI/LobbySearchResultWidget.h"

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
	// some loading animation
}

void UMainMenuWidget::OnFindAllSessionsFinished(bool bWasSuccessful, const FOnlineSessionSearch_Rogue& SearchResults)
{
	for (const FOnlineSession_Rogue& OnlineSessionSearchResult : SearchResults.Sessions)
	{
		TObjectPtr<ULobbySearchResultWidget> LobbySearchResultWidget = (CreateWidget<ULobbySearchResultWidget>(VB_SearchResult, LobbySearchResultWidgetClass->StaticClass()));
		if (LobbySearchResultWidget)
		{
			LobbySearchResultWidget->Init(OnlineSessionSearchResult);
		}
	}
}


