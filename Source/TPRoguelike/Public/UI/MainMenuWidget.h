// Made by Jan Puto 2023 :D

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Subsystems/EOSSubsystem.h"
#include "MainMenuWidget.generated.h"

class UMenuButtonWidget;
class UVerticalBox;
//class UEOSSubsystem;

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
	TObjectPtr<UMenuButtonWidget> JoinGameButton;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Rogue|Widgets", meta = (BindWidget))
	TObjectPtr<UMenuButtonWidget> SettingsButton;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Rogue|Widgets", meta = (BindWidget))
	TObjectPtr<UMenuButtonWidget> QuitGameButton;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Rogue|Widgets", meta = (BindWidget))
	TObjectPtr<UVerticalBox> VB_SearchResult;

protected:
	virtual void NativeConstruct() override;

	void OnFindAllSessionsStarted();

	void OnFindAllSessionsFinished(bool bWasSuccessful, const FOnlineSessionSearch_Rogue& SearchResults);

	UPROPERTY()
	TObjectPtr<UEOSSubsystem> EOSSubsystem;
};
