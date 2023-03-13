// Made by Jan Puto 2023 :D

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "OnlineSessionSettings.h"
#include "Online/CoreOnlineFwd.h"
#include "Online/OnlineSessionSearch.h"
#include "EOSSubsystem.generated.h"

class IOnlineSubsystem;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnFindAllSessionsStarted);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnFindAllSessionsFinished, bool, bWasSuccessful, const FOnlineSessionSearch_Rogue&, SearchResults);


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
	void CreateSession(FName SessionName = "TestSessionName", int32 MaxPlayers = 4);
	void OnCreateSessionComplete(FName SessionName, bool bWasSuccessful);

	UFUNCTION(BlueprintCallable)
	void DestroySession(FName SessionName = "TestSessionName");
	void OnDestroySessionComplete(FName SessionName, bool bWasSuccessful);

	UFUNCTION(BlueprintCallable)
	void FindSessionByName(FName SessionName = "TestSessionName", bool bShouldJoinIfSessionFound = true);
	void OnFindSessionByNameComplete(bool bWasSuccessful);

	UFUNCTION(BlueprintCallable)
	void FindAllSessions();
	void OnFindAllSessionsComplete(bool bWasSuccessful);
	UPROPERTY(BlueprintAssignable);
	FOnFindAllSessionsStarted OnFindAllSessionStarted;
	UPROPERTY(BlueprintAssignable);
	FOnFindAllSessionsFinished OnFindAllSessionFinished;

	//UFUNCTION(BlueprintCallable)
	//void JoinSession(FName SessionName, const FOnlineSessionSearchResult& DesiredSession);

	void JoinSessionByOwnerID(FName SessionName, FUniqueNetIdPtr _OwningUserId);
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

protected:
	UFUNCTION()
	FORCEINLINE bool IsLoggedIn() const { return bIsLoggedIn && OnlineSubsystem; }

	FOnlineSessionSearchResult GetAlreadyFoundSessionByOwnerID(FUniqueNetIdPtr _OwningUserId);

	UPROPERTY()
	bool bShouldJoinIfSessionFoundByName = false;

	UPROPERTY()
	bool bIsLoggedIn;

	UPROPERTY()
	FName TestSessionName = FName("TestSessionName");

	UPROPERTY()
	FString TestSessionSearchKeyword = "TestSessionSearchKeyword";

	TSharedPtr<FOnlineSessionSearch> SearchSettings;

	IOnlineSubsystem* OnlineSubsystem;
};
