// Made by Jan Puto 2022 :D

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SAttributeComponent.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnHealthChanged, AActor*, InstigatorActor,  class USAttributeComponent*, OwningComp, float, NewHealth, float, HealthDelta);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TPROGUELIKE_API USAttributeComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	
	UFUNCTION(BlueprintCallable, Category = "Attributes")
	static USAttributeComponent* GetAttributeComponent(AActor* FromActor);

	// Returns false is Actor doesn`t have AttributeComponent
	UFUNCTION(BlueprintCallable, Category = "Attributes|Health")
	static bool IsActorAlive(AActor* Actor);

	USAttributeComponent();

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attributes|Health")
	float Health = 100.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attributes|Health")
	float MaxHealth = 100.f;

	// Credits that will be granted to a killer of component owner
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attributes|Credits")
	int32 CreditsForKilling = 1;

	// HealthMax, Stamina, Mana, CritChance;

public:
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Attributes|Health")
	bool IsAlive() const; 

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Attributes|Health")
	bool IsFullyHealed() const;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Attributes|Health")
	bool IsHealthHigherThen(float IsHealthHigherThenThis) const;

	UFUNCTION(BlueprintCallable, Category = "Attributes|Health")
	bool Kill(AActor* InstigatorActor);

	UPROPERTY(BlueprintAssignable)
	FOnHealthChanged OnHealthChanged;

	UFUNCTION(BlueprintCallable,  Category = "Attributes")
	bool ApplyHealthChange(AActor* InstigatorActor, float HealthDelta);

	UFUNCTION(BlueprintCallable, Category = "Attributes|Credits")
	FORCEINLINE int32 GetCreditsAmountForKill() const { return CreditsForKilling; };
	
};
