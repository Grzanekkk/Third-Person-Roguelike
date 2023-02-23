// Made by Jan Puto 2022 :D

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameplayTagContainer.h"
#include "SActionComponent.generated.h"

class USAction;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TPROGUELIKE_API USActionComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	USActionComponent();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	/** We need this to be able to replicate SActions */
	virtual bool ReplicateSubobjects(class UActorChannel* Channel, class FOutBunch* Bunch, FReplicationFlags* RepFlags) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rogue|Action|Tags")
	FGameplayTagContainer ActiveGameplayTags;

	UFUNCTION(BlueprintCallable, Category = "Rogue|Action")
	void AddAction(AActor* Instigator, TSubclassOf<USAction> ActionClass);

	UFUNCTION(BlueprintCallable, Category = "Rogue|Action")
	void RemoveAction(USAction* Action);

	UFUNCTION(BlueprintCallable, Category = "Rogue|Action")
	bool StartActionByName(AActor* Instigator, FName ActionName);

	UFUNCTION(BlueprintCallable, Category = "Rogue|Action")
	bool StopActionByName(AActor* Instigator, FName ActionName);

	UPROPERTY(EditDefaultsOnly, Category = "Rogue|Action")
	TArray<TSubclassOf<USAction>> DefaultActions;

	UFUNCTION(Server, Reliable)
	void ServerStartActionByName(AActor* Instigator, FName ActionName);

	UFUNCTION(Server, Reliable)
	void ServerStopActionByName(AActor* Instigator, FName ActionName);

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void DrawAllActionsOnScreen();

	UPROPERTY(Replicated)
	TArray<TObjectPtr<USAction>> Actions;

};
