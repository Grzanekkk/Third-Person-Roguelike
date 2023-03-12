// Made by Jan Puto 2023 :D

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Online/OnlineSessionSearch.h"
#include "LobbySearchResultWidget.generated.h"

class UMenuButtonWidget;
class UHorizontalBox;
class UTextBlock;
/**
 * 
 */
UCLASS()
class TPROGUELIKE_API ULobbySearchResultWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION()
	void Init(const FOnlineSession_Rogue& _OnlineSessionSearchResult);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Rogue|Widgets", meta = (BindWidget))
	TObjectPtr<UHorizontalBox> HB_Main;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Rogue|Widgets", meta = (BindWidget))
	TObjectPtr<UMenuButtonWidget> B_JoinLobby;

	UPROPERTY(BlueprintReadOnly, Category = "Rogue|Widgets", meta = (BindWidget))
	TObjectPtr<UTextBlock> T_LobbyName;

	UPROPERTY(BlueprintReadOnly, Category = "Rogue|Widgets", meta = (BindWidget))
	TObjectPtr<UTextBlock> T_SessionOwnerName;

	UPROPERTY(BlueprintReadOnly, Category = "Rogue|Widgets", meta = (BindWidget))
	TObjectPtr<UTextBlock> T_PlayerSlots;

protected:
	UFUNCTION()
	void OnJoinLobbyButtonClicked();

	FOnlineSessionSearchResult* OnlineSessionSearchResult;

	UPROPERTY()
	FString SessionLevelName;
};
