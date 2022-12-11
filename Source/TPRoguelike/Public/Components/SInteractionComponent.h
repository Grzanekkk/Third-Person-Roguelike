// Made by Jan Puto 2022 :D

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SInteractionComponent.generated.h"

class USWorldUserWidget;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TPROGUELIKE_API USInteractionComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	USInteractionComponent();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION()
	void PrimaryInteract();

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void FindBestInteractable();

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Rogue|Interaction")
	TObjectPtr<AActor> FocusedActor = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Rogue|Interaction|UI")
	TSubclassOf<USWorldUserWidget> DefaultWidgetClass = nullptr;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Rogue|Interaction|UI")
	TObjectPtr<USWorldUserWidget> WidgetInstance = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Rogue|Interaction|Trace")
	float InteractionDistance = 1000.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Rogue|Interaction|Trace")
	float InteractionRadius = 30.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Rogue|Interaction|Trace")
	TEnumAsByte<ECollisionChannel> CollisionChannel = ECC_WorldDynamic;
};
