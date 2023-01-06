// Made by Jan Puto 2022 :D

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "SPlayerState.generated.h"

class USSaveGame;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnCreditsChanged, AActor*, InstigatorActor, float, NewCredits, float, CreditsDelta);


UCLASS()
class TPROGUELIKE_API ASPlayerState : public APlayerState
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, Category = "Rogue|Credits")
	bool ApplyCreditsChange(int32 CreditsDelta);

	UFUNCTION(NetMulticast, Reliable)
	void MulticastOnCreditsChanged(AActor* InstigatorActor, float NewCredits, float CreditsDelta);

	UFUNCTION(BlueprintCallable, Category = "Rogue|Credits")
	FORCEINLINE int32 GetCredits() const { return AvalibleCredits; };

	UPROPERTY(BlueprintAssignable)
	FOnCreditsChanged OnCreditsChanged;

	UFUNCTION(BlueprintCallable, Category = "Rogue|Credits")
	void SavePlayerState(USSaveGame* SaveObject);

	UFUNCTION(BlueprintCallable, Category = "Rogue|Credits")
	void LoadPlayerState(USSaveGame* SaveObject);

protected:
	UPROPERTY(Replicated, EditDefaultsOnly, Category = "Rogue|Credits")
	int32 AvalibleCredits = 0;
};