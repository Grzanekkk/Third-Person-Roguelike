// Made by Jan Puto 2022 :D

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameplayTagContainer.h"
#include "SAttributeComponent.generated.h"


//DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnHealthChanged, AActor*, InstigatorActor,  class USAttributeComponent*, OwningComp, float, NewHealth, float, HealthDelta);
//DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnRageChanged, class USAttributeComponent*, OwningComp, float, NewRage, float, RageDelta);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnAttributeChanged, AActor*, InstigatorActor, class USAttributeComponent*, OwningComp, float, NewValue, float, Delta);


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TPROGUELIKE_API USAttributeComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	
	UFUNCTION(BlueprintCallable, Category = "Rogue|Attributes")
	static USAttributeComponent* GetAttributeComponent(AActor* FromActor);

	// Returns false is Actor doesn`t have AttributeComponent
	UFUNCTION(BlueprintCallable, Category = "Rogue|Attributes|Health")
	static bool IsActorAlive(AActor* Actor);

	USAttributeComponent();

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Replicated, Category = "Rogue|Attributes|Health")
	float CurrentHealth = 100.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Replicated, Category = "Rogue|Attributes|Health")
	float MaxHealth = 100.f;

	// Credits that will be granted to a killer of component owner
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Rogue|Attributes|Credits")
	int32 CreditsForKilling = 1;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Rogue|Attributes|Rage")
	float CurrentRage = 10.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Rogue|Attributes|Rage")
	float MaxRage = 100.0f;

	// Amount of Rage gained per 1 damage point recived
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Rogue|Attributes|Rage")
	float RageMultiplier = 0.2f;

	// HealthMax, Stamina, Mana, CritChance;

public:
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Rogue|Attributes|Health")
	bool IsAlive() const; 

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Rogue|Attributes|Health")
	bool IsFullyHealed() const;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Rogue|Attributes|Health")
	bool IsHealthHigherThen(float IsHealthHigherThenThis) const;

	UFUNCTION(BlueprintCallable, Category = "Rogue|Attributes|Health")
	bool Kill(AActor* InstigatorActor);

	UPROPERTY(BlueprintAssignable, Category = "Rogue|Attributes|Health")
	FOnAttributeChanged OnHealthChanged;

	UFUNCTION(NetMulticast, Reliable)
	void MulticastOnHealthChanged(AActor* InstigatorActor, USAttributeComponent* OwningComp, float NewHealth, float HealthDelta);

	UFUNCTION(BlueprintCallable,  Category = "Rogue|Attributes|Health")
	bool ApplyHealthChange(AActor* InstigatorActor, float HealthDelta);

	UPROPERTY(BlueprintAssignable, Category = "Rogue|Attributes|Rage")
	FOnAttributeChanged OnRageChanged;

	UFUNCTION(BlueprintCallable, Category = "Rogue|Attributes|Rage")
	bool ApplyRageChange(AActor* InstigatorActor, float RageDelta);

	UFUNCTION(BlueprintCallable, Category = "Rogue|Attributes|Credits")
	FORCEINLINE int32 GetCreditsAmountForKill() const { return CreditsForKilling; };
	
	/// Quests
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rogue|Quests|Tags")
	FGameplayTagContainer ObjectivesAffectedOnDeath;
};
