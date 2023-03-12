// Made by Jan Puto 2023 :D

#pragma once

#include "CoreMinimal.h"
#include "OnlineSessionSettings.h"
#include "OnlineSessionSearch.generated.h"


/**
 *
 */
 // Copy of FOnlineSession, but as a USTRUCT
USTRUCT()
struct FOnlineSession_Rogue
{
	GENERATED_BODY()

public:
	/** Owner of the session */
	FUniqueNetIdPtr OwningUserId;
	/** Owner name of the session */
	FString OwningUserName;
	/** The settings associated with this session */
	FOnlineSessionSettings SessionSettings;
	/** The platform specific session information */
	TSharedPtr<class FOnlineSessionInfo> SessionInfo;
	/** The number of private connections that are available (read only) */
	int32 NumOpenPrivateConnections;
	/** The number of publicly available connections that are available (read only) */
	int32 NumOpenPublicConnections;

	/** Default constructor, used when serializing a network packet */
	FOnlineSession_Rogue() :
		OwningUserId(nullptr),
		SessionInfo(nullptr),
		NumOpenPrivateConnections(0),
		NumOpenPublicConnections(0)
	{
	}
};

/**
 *
 */
USTRUCT()
struct FOnlineSessionSearch_Rogue
{
	GENERATED_BODY()

public:
	TArray<FOnlineSession_Rogue> Sessions;

	FOnlineSessionSearch_Rogue() { }

	FOnlineSessionSearch_Rogue(TArray<FOnlineSession_Rogue> _Sessions)
	{
		Sessions = _Sessions;
	}
};

