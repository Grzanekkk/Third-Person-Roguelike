// Made by Jan Puto 2022 :D

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GameplayTagContainer.h"
#include "SAction.generated.h"

class UWorld;
class USActionComponent;

UCLASS(Blueprintable)
class TPROGUELIKE_API USAction : public UObject
{
	GENERATED_BODY()
	
public:

	UFUNCTION(BlueprintNativeEvent, Category = "Action")
	void StartAction(AActor* Instigator);

	UFUNCTION(BlueprintNativeEvent, Category = "Action")
	void StopAction(AActor* Instigator);

	UFUNCTION(BlueprintNativeEvent, Category = "Action")
	bool CanStart(AActor* Instigator);

	UFUNCTION(BlueprintCallable, Category = "Action")
	FORCEINLINE	bool IsRunning() const { return bIsRunning; };

	UPROPERTY(EditDefaultsOnly, Category = "Action")
	FName ActionName = "";

	UPROPERTY(EditDefaultsOnly, Category = "Action")
	bool bAutoStart = false;

	// We have to override this function so we will have acces to line traces and other world related stuff in blueprint children of this class
	UWorld* GetWorld() const override;

protected:
	UFUNCTION(BlueprintCallable, Category = "Action")
	USActionComponent* GetOuterComponent() const;

	// Tags Added to owning actor when activated, removed when action stops
	UPROPERTY(EditDefaultsOnly, Category = "Action|Tags")
	FGameplayTagContainer GrantsTags;

	// Action acn olny atart if Owning Actor has none of these Tags appplied
	UPROPERTY(EditDefaultsOnly, Category = "Action|Tags")
	FGameplayTagContainer BlockedTags;

	UPROPERTY()
	bool bIsRunning = false;
};
