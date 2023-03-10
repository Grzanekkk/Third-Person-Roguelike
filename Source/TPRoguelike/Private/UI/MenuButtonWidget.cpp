// Made by Jan Puto 2023 :D


#include "UI/MenuButtonWidget.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"

//void UMenuButtonWidget::PreConstruct(UUserWidget* UserWidget, bool IsDesignTime)
//{
//	//Super::PreConstruct(IsDesignTime);
//	//
//	//ButtonTextBox->SetText(ButtonText);
//	//
//	//MainButton->OnClicked.AddDynamic(this, &UMenuButtonWidget::OnButtonClicked);
//}

void UMenuButtonWidget::OnButtonClicked()
{
	OnClickedEvent.Broadcast();
}
