// Made by Jan Puto 2023 :D

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "EOSSubsystem.generated.h"

class IOnlineSubsystem;

/**
 * 
 */
UCLASS()
class TPROGUELIKE_API UEOSSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	void Login();

	UFUNCTION(BlueprintCallable)
	void CreateSession();

	void OnCreateSessionComplete(FName SessionName, bool bWasSuccessfull);

	void OnLoginComplete(int ControllerIndex, bool bWasSuccessful, const FUniqueNetId& UserId, const FString& ErrorString);

protected:
	IOnlineSubsystem* OnlineSubsystem;

	UPROPERTY()
	bool bIsLoggedIn;
};
