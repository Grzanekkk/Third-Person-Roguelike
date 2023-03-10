// Made by Jan Puto 2023 :D

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "OnlineSessionSettings.h"
#include "EOSSubsystem.generated.h"

class IOnlineSubsystem;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnFindSessionStarted);
//DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnFindSessionFinished, bool, bWasSuccessful, FOnlineSessionSearch, SearchResults);


/**
 * 
 */
UCLASS()
class TPROGUELIKE_API UEOSSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	UFUNCTION(BlueprintCallable)
	void CreateSession();
	void OnCreateSessionComplete(FName SessionName, bool bWasSuccessful);

	UFUNCTION(BlueprintCallable)
	void DestroySession();
	void OnDestroySessionComplete(FName SessionName, bool bWasSuccessful);

	UFUNCTION(BlueprintCallable)
	void FindSession();
	void OnFindSessionComplete(bool bWasSuccessful);

	void OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result);

	void Login();
	void OnLoginComplete(int ControllerIndex, bool bWasSuccessful, const FUniqueNetId& UserId, const FString& ErrorString);

	UFUNCTION(BlueprintCallable)
	void GetAllFriends();
	void OnGetAllFriesdsComplete(int32 LocalUserNum, bool bWasSuccessful, const FString& ListName, const FString& ErrorStr);

	UFUNCTION(BlueprintCallable)
	void ShowInviteUI();
	UFUNCTION(BlueprintCallable)
	void ShowFriendsUI();

	UPROPERTY(BlueprintAssignable);
	FOnFindSessionStarted OnFindSessionStarted;

	//UPROPERTY(BlueprintAssignable);
	//FOnFindSessionFinished OnFindSessionFinished;

protected:
	IOnlineSubsystem* OnlineSubsystem;

	UPROPERTY()
	bool bIsLoggedIn;

	UPROPERTY()
	FName TestSessionName = FName("TestSession");

	TSharedPtr<FOnlineSessionSearch> SearchSettings;
};
