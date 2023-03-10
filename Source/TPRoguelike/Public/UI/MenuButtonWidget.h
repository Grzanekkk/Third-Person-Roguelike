// Made by Jan Puto 2023 :D

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MenuButtonWidget.generated.h"

class UButton;
class UTextBlock;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnClickedEvent);

/**
 * 
 */
UCLASS()
class TPROGUELIKE_API UMenuButtonWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	//virtual void PreConstruct(UUserWidget* UserWidget, bool IsDesignTime) override;

	UPROPERTY(BlueprintAssignable, Category = "Rogue|Widgets|Events")
	FOnClickedEvent OnClickedEvent;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UButton> MainButton;

	UPROPERTY(BlueprintReadOnly, Category = "Rogue|Widgets", meta = (BindWidget))
	TObjectPtr<UTextBlock> ButtonTextBox;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Rogue|Settings")
	FText ButtonText = FText::FromString("Default Text");

	UFUNCTION(BlueprintCallable, Category = "Rogue|Widgets|Events")
	void OnButtonClicked();
};
