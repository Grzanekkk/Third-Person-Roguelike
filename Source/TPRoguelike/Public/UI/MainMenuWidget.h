// Made by Jan Puto 2023 :D

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Online/OnlineSessionSearch.h"
#include "MainMenuWidget.generated.h"

class UMenuButtonWidget;
class UVerticalBox;
class ULobbySearchResultWidget;
class UTextBlock;
class UEOSSubsystem;

/**
 * 
 */
UCLASS()
class TPROGUELIKE_API UMainMenuWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Rogue|Widgets", meta = (BindWidget))
	TObjectPtr<UVerticalBox> VB_Buttons;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Rogue|Widgets", meta = (BindWidget))
	TObjectPtr<UMenuButtonWidget> StartSinglePlayerGameButton;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Rogue|Widgets", meta = (BindWidget))
	TObjectPtr<UMenuButtonWidget> HostGameButton;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Rogue|Widgets", meta = (BindWidget))
	TObjectPtr<UMenuButtonWidget> JoinLocalGameButton;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Rogue|Widgets", meta = (BindWidget))
	TObjectPtr<UMenuButtonWidget> SettingsButton;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Rogue|Widgets", meta = (BindWidget))
	TObjectPtr<UMenuButtonWidget> QuitGameButton;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Rogue|Widgets", meta = (BindWidget))
	TObjectPtr<UVerticalBox> VB_SearchResult;

	UPROPERTY(BlueprintReadOnly, Category = "Rogue|Widgets", meta = (BindWidget))
	TObjectPtr<UTextBlock> T_Loading;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Rogue|Widgets")
	TSubclassOf<ULobbySearchResultWidget> LobbySearchResultWidgetClass;

protected:
	virtual void NativeConstruct() override;

	UFUNCTION()
	void OnFindAllSessionsStarted();

	UFUNCTION()
	void OnFindAllSessionsFinished(bool bWasSuccessful, const FOnlineSessionSearch_Rogue& SearchResults);

	UPROPERTY()
	TObjectPtr<UEOSSubsystem> EOSSubsystem;
};
