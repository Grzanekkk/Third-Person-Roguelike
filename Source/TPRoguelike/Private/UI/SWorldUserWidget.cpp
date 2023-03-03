// Made by Jan Puto 2022 :D


#include "UI/SWorldUserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/SizeBox.h"
#include "FunctionLibrary/LogsFunctionLibrary.h"

void USWorldUserWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (!IsValid(AttachedActor))
	{
		RemoveFromParent();

		FString Msg = FString::Printf(TEXT("AttachedActor is no longer valid. Removing %s widget from viewport"), *this->GetName());
		ULogsFunctionLibrary::LogOnScreen(GetWorld(), Msg, ERogueLogCategory::WARNING);

		return;
	}

	FVector2D OnScreenPosition;
	if (UGameplayStatics::ProjectWorldToScreen(GetOwningPlayer(), AttachedActor->GetActorLocation() + WorldOffset, OnScreenPosition))
	{
		//Returns false if coundnot find position on the screen

		float Scale = UWidgetLayoutLibrary::GetViewportScale(this);

		OnScreenPosition /= Scale;

		if (ParentSizeBox)
		{
			ParentSizeBox->SetRenderTranslation(OnScreenPosition);
		}
	}

}