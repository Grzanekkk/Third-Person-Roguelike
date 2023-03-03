// Made by Jan Puto 2023 :D


#include "Subsystems/EOSSubsystem.h"
#include "OnlineSubsystem.h"
#include "OnlineSessionSettings.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "FunctionLibrary/LogsFunctionLibrary.h"

void UEOSSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	OnlineSubsystem = IOnlineSubsystem::Get();
}

void UEOSSubsystem::CreateSession()
{
	if (OnlineSubsystem)
	{
		if (IOnlineSessionPtr SessionPtr = OnlineSubsystem->GetSessionInterface())
		{
			FOnlineSessionSettings SessionSettings;
			SessionSettings.bIsDedicated = false;
			SessionSettings.bAllowJoinInProgress = true;
			SessionSettings.bAllowJoinViaPresence = true;
			SessionSettings.bIsLANMatch = true; // Might remove
			SessionSettings.bShouldAdvertise = true;
			SessionSettings.NumPublicConnections = 5;
			SessionSettings.bUsesPresence = true;

			SessionPtr->OnCreateSessionCompleteDelegates.AddUObject(this, &UEOSSubsystem::OnCreateSessionComplete);
			SessionPtr->CreateSession(0, "Test Session", SessionSettings);
		}
	}
}

void UEOSSubsystem::OnCreateSessionComplete(FName SessionName, bool bWasSuccessfull)
{
	FString Msg = bWasSuccessfull ? "Session created successfully" : "Failed to create session!";
	ERogueLogCategory LogCategory = bWasSuccessfull ? ERogueLogCategory::SUCCESS : ERogueLogCategory::ERROR;
	ULogsFunctionLibrary::LogOnScreen(GetWorld(), Msg, LogCategory);

	if (OnlineSubsystem)
	{
		if (IOnlineSessionPtr SessionPtr = OnlineSubsystem->GetSessionInterface())
		{
			SessionPtr->ClearOnCreateSessionCompleteDelegates(this);
		}
	}
}
