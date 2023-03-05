// Made by Jan Puto 2023 :D


#include "Subsystems/EOSSubsystem.h"
#include "OnlineSubsystem.h"
#include "OnlineSessionSettings.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "Interfaces/OnlineIdentityInterface.h"
#include "FunctionLibrary/LogsFunctionLibrary.h"

void UEOSSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	bIsLoggedIn = false;

	OnlineSubsystem = IOnlineSubsystem::Get();
	Login();
}

void UEOSSubsystem::Login()
{
	if (OnlineSubsystem)
	{
		if (IOnlineIdentityPtr Identity = OnlineSubsystem->GetIdentityInterface())
		{
			FOnlineAccountCredentials Credentials;
			//Credentials.Id = FString();
			//Credentials.Token = FString();
			//Credentials.Type = FString("accountportal");
			
			Credentials.Id = FString("127.0.0.1:8081");
			Credentials.Token = FString("BobBobowski");
			Credentials.Type = FString("developer");

			Identity->OnLoginCompleteDelegates->AddUObject(this, &UEOSSubsystem::OnLoginComplete);
			Identity->Login(0, Credentials);
		}
	}
}

void UEOSSubsystem::CreateSession()
{
	if (bIsLoggedIn)
	{
		if (OnlineSubsystem)
		{
			if (IOnlineSessionPtr SessionPtr = OnlineSubsystem->GetSessionInterface())
			{
				FOnlineSessionSettings SessionSettings;
				SessionSettings.bIsDedicated = false;
				SessionSettings.bAllowJoinInProgress = true;
				SessionSettings.bAllowJoinViaPresence = true;
				SessionSettings.bIsLANMatch = false; 
				SessionSettings.bShouldAdvertise = true;
				SessionSettings.NumPublicConnections = 5;
				SessionSettings.bUsesPresence = true;
				SessionSettings.Set()

				SessionPtr->OnCreateSessionCompleteDelegates.AddUObject(this, &UEOSSubsystem::OnCreateSessionComplete);
				SessionPtr->CreateSession(0, "Test Session", SessionSettings);
			}
		}
	}
	else
	{
		ULogsFunctionLibrary::LogOnScreen(GetWorld(), "You need to be logged in to create a session", ERogueLogCategory::WARNING);
	}
}

void UEOSSubsystem::OnCreateSessionComplete(FName SessionName, bool bWasSuccessful)
{
	FString Msg = bWasSuccessful ? "Session created successfully" : "Failed to create session!";
	ERogueLogCategory LogCategory = bWasSuccessful ? ERogueLogCategory::SUCCESS : ERogueLogCategory::ERROR;
	ULogsFunctionLibrary::LogOnScreen(GetWorld(), Msg, LogCategory);

	if (OnlineSubsystem)
	{
		if (IOnlineSessionPtr SessionPtr = OnlineSubsystem->GetSessionInterface())
		{
			SessionPtr->ClearOnCreateSessionCompleteDelegates(this);
		}
	}
}

void UEOSSubsystem::OnLoginComplete(int ControllerIndex, bool bWasSuccessful, const FUniqueNetId& UserId, const FString& ErrorString)
{
	FString Msg = bWasSuccessful ? "LoggedIn successfully" : "Failed to Login!";
	ERogueLogCategory LogCategory = bWasSuccessful ? ERogueLogCategory::SUCCESS : ERogueLogCategory::ERROR;
	ULogsFunctionLibrary::LogOnScreen(GetWorld(), Msg, LogCategory);

	bIsLoggedIn = true;

	if (OnlineSubsystem)
	{
		if (IOnlineIdentityPtr Identity = OnlineSubsystem->GetIdentityInterface())
		{
			Identity->ClearOnLoginCompleteDelegates(0, this);
		}
	}
}
