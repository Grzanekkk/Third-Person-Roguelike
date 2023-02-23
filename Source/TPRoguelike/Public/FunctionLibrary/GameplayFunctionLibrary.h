// Made by Jan Puto 2022 :D

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "GameplayFunctionLibrary.generated.h"

/**
 * 
 */
UCLASS()
class TPROGUELIKE_API UGameplayFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, Category = "Rogue|Gameplay")
	static bool ApplyDamage(AActor* DamageCauser, AActor* TargetActor, float DamageAmount);

	UFUNCTION(BlueprintCallable, Category = "Rogue|Gameplay")
	static bool ApplyDirectionalDamage(AActor* DamageCauser, AActor* TargetActor, float DamageAmount, const FHitResult& HitResult);

	UFUNCTION(BlueprintCallable, Category = "Rogue|Gameplay")
	static bool AddObjectiveStat(const UObject* WorldContextObject, FGameplayTag ObjectiveTag, int32 Stat);
};
