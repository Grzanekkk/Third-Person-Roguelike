// Made by Jan Puto 2022 :D

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "SPlayerState.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnCreditsChanged, AActor*, InstigatorActor, float, NewCredits, float, CreditsDelta);


UCLASS()
class TPROGUELIKE_API ASPlayerState : public APlayerState
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, Category = "Credits")
	bool AddCredits(int32 CreditsDelta);

	UPROPERTY(BlueprintAssignable)
	FOnCreditsChanged OnCreditsChanged;

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Credits")
	int32 AvalibleCredits = 0;
};
