// Made by Jan Puto 2022 :D

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "SGameplayInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class USGameplayInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class TPROGUELIKE_API ISGameplayInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void Interact(APawn* Instigator);

	// Here we are checking if player meets requierements to interact fe: has enough coins or is damaged
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	bool CanInteract(APawn* Instigator);

	// Here we are checking if interactable object itself is capable of beeing interacted fe: is not on cooldown
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	bool IsEnabled();
};
