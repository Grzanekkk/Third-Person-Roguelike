// Made by Jan Puto 2023 :D

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LobbySearchResultWidget.generated.h"

class UMenuButtonWidget;
class UHorizontalBox;
/**
 * 
 */
UCLASS()
class TPROGUELIKE_API ULobbySearchResultWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Rogue|Widgets", meta = (BindWidget))
	TObjectPtr<UHorizontalBox> HB_Buttons;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Rogue|Widgets", meta = (BindWidget))
	TObjectPtr<UMenuButtonWidget> B_JoinLobby;
};
