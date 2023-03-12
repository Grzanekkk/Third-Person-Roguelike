// Made by Jan Puto 2023 :D


#include "UI/LobbySearchResultWidget.h"
#include "UI/MenuButtonWidget.h"
#include "Components/TextBlock.h"
#include "OnlineSessionSettings.h"
#include "OnlineSubsystemTypes.h"

void ULobbySearchResultWidget::Init(const FOnlineSession_Rogue& _OnlineSessionSearchResult)
{
	T_SessionOwnerName->SetText(FText::FromString(_OnlineSessionSearchResult.OwningUserName));
	_OnlineSessionSearchResult.SessionSettings.Get(SETTING_MAPNAME, SessionLevelName);
	//T_LobbyName->SetText()
	B_JoinLobby->OnClickedEvent.AddDynamic(this, &ULobbySearchResultWidget::OnJoinLobbyButtonClicked);
}

void ULobbySearchResultWidget::OnJoinLobbyButtonClicked()
{
}
