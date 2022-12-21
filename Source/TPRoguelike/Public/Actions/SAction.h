// Made by Jan Puto 2022 :D

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GameplayTagContainer.h"
#include "SAction.generated.h"

class UWorld;
class USActionComponent;

/**
*
**/
USTRUCT()
struct FReplicationData
{
	GENERATED_BODY()

public:
	UPROPERTY()
	bool bIsRunning = false;

	UPROPERTY()
	TObjectPtr<AActor> Instigator = nullptr;
};

/**
*
**/
UCLASS(Blueprintable)
class TPROGUELIKE_API USAction : public UObject
{
	GENERATED_BODY()
	
public:

	UFUNCTION()
	void Initialize(USActionComponent* NewActionComponent);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Rogue|Action")
	void StartAction(AActor* Instigator);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Rogue|Action")
	void StopAction(AActor* Instigator);

	UFUNCTION(BlueprintNativeEvent, Category = "Rogue|Action")
	bool CanStart(AActor* Instigator);

	UFUNCTION(BlueprintCallable, Category = "Rogue|Action")
	FORCEINLINE	bool IsRunning() const { return ReplicationData.bIsRunning; };

	UPROPERTY(EditDefaultsOnly, Category = "Rogue|Action")
	FName ActionName = "";

	UPROPERTY(EditDefaultsOnly, Category = "Rogue|Action")
	bool bAutoStart = false;

	/** We have to override this function so we will have acces to line traces and other world related stuff in blueprint children of this class */
	UWorld* GetWorld() const override;

	/** We need to override this function so we can use actions in the network fe.replicate them */
	virtual bool IsSupportedForNetworking() const override;

protected:
	UFUNCTION(BlueprintCallable, Category = "Rogue|Action")
	USActionComponent* GetOwningComponent() const;

	UPROPERTY(Replicated)
	TObjectPtr<USActionComponent> OwningActionComponent = nullptr;

	/** Tags Added to owning actor when activated, removed when action stops */
	UPROPERTY(EditDefaultsOnly, Category = "Rogue|Action|Tags")
	FGameplayTagContainer GrantsTags;

	/** Action acn olny atart if Owning Actor has none of these Tags appplied */
	UPROPERTY(EditDefaultsOnly, Category = "Rogue|Action|Tags")
	FGameplayTagContainer BlockedTags;

	UPROPERTY(ReplicatedUsing = "OnRep_ReplicatoinData" /*Category = "Rogue|Action"*/)
	FReplicationData ReplicationData;

	UFUNCTION()
	void OnRep_ReplicatoinData();
};