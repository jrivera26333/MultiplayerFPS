// Fill out your copyright notice in the Description page of Project Settings.


#include "MultiplayerFPSGameInstance.h"
#include "OnlineSubsystem.h"
#include "OnlineSessionSettings.h"
#include "Interfaces/OnlineSessionInterface.h"

const static FName SESSION_NAME = TEXT("My Session Game");

//All Clients have a GameInstance often used for persistent data
void UMultiplayerFPSGameInstance::Init()
{
	Super::Init();

	//Getting the current Subsystem. (Null by default unless specified in the DefaultEngine.ini file
	static IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get();
	if (Subsystem != nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Subsystem: %s"), *Subsystem->GetSubsystemName().ToString());

		//Interface Regarding the Management Services
		SessionInterface = Subsystem->GetSessionInterface();

		//IsValid() for SharedPtrs
		if (SessionInterface.IsValid())
		{
			//Callbacks
			SessionInterface->OnCreateSessionCompleteDelegates.AddUObject(this, &UMultiplayerFPSGameInstance::OnCreateSessionComplete);
			SessionInterface->OnDestroySessionCompleteDelegates.AddUObject(this, &UMultiplayerFPSGameInstance::OnDestroySessionComplete);
			SessionInterface->OnFindSessionsCompleteDelegates.AddUObject(this, &UMultiplayerFPSGameInstance::OnFindSessionsComplete);

			//Casting SharedPtr to SharedRef
			SessionSearch = MakeShareable(new FOnlineSessionSearch());
			if (SessionSearch.IsValid())
			{
				SessionSearch->bIsLanQuery = true; //Looking over Local

				UE_LOG(LogTemp, Warning, TEXT("Starting to find sessions"));
				SessionInterface->FindSessions(0, SessionSearch.ToSharedRef());
			}
		}
	}
	else
		UE_LOG(LogTemp, Warning, TEXT("Subsystem is not null!"));
}

void UMultiplayerFPSGameInstance::Host()
{
	if (SessionInterface.IsValid())
	{
		auto ExistingSession = SessionInterface->GetNamedSession(SESSION_NAME);
		if (ExistingSession != nullptr)
		{
			//We have a delegate that will create our session once the deletion has happened. OnDestroySessionComplete()
			SessionInterface->DestroySession(SESSION_NAME);
		}
		else
		{
			CreateSession();
		}
	}
}

void UMultiplayerFPSGameInstance::CreateSession()
{
	if (SessionInterface.IsValid())
	{

		FOnlineSessionSettings SessionSettings;

		if (IOnlineSubsystem::Get()->GetSubsystemName() == "NULL")
		{
			//Local
			SessionSettings.bIsLANMatch = true;
		}
		else
		{
			SessionSettings.bIsLANMatch = false;
		}

		SessionSettings.NumPublicConnections = 2;
		SessionSettings.bShouldAdvertise = true; //Be able to present itself

		//Async Function so we will use a delegate. OnCreateSessionComplete()
		SessionInterface->CreateSession(0, SESSION_NAME, SessionSettings);
	}
}

void UMultiplayerFPSGameInstance::OnDestroySessionComplete(FName SessionName, bool Success)
{
	if (Success)
	{
		CreateSession();
	}
}

void UMultiplayerFPSGameInstance::OnFindSessionsComplete(bool Success)
{
	if (Success && SessionSearch.IsValid())
	{
		UE_LOG(LogTemp, Warning, TEXT("Finished find sessions"));

		for (const FOnlineSessionSearchResult& NamedResult : SessionSearch->SearchResults)
		{
			UE_LOG(LogTemp, Warning, TEXT("Found Session: %s"), *NamedResult.Session.GetSessionIdStr());
		}
	}
}

void UMultiplayerFPSGameInstance::OnCreateSessionComplete(FName SessionName, bool Success)
{
	if (!Success)
	{
		UE_LOG(LogTemp, Warning, TEXT("Could not create session"));
		return;
	}

	UEngine* Engine = GetEngine();
	if (Engine == nullptr) return;

	Engine->AddOnScreenDebugMessage(0, 2, FColor::Green, TEXT("Hosting"));

	UWorld* World = GetWorld();
	if (World == nullptr) return;

	//Will jump the server to a new level and all connected clients will follow. Server will call APlayerController::ClientTravel for all Client Players connected
	World->ServerTravel("/Game/Maps/Lobby?listen");
}

void UMultiplayerFPSGameInstance::DebugSession()
{
	UE_LOG(LogTemp, Warning, TEXT("Session Debug"));
}