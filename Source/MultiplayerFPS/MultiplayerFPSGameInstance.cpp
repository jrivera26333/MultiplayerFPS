// Fill out your copyright notice in the Description page of Project Settings.


#include "MultiplayerFPSGameInstance.h"
#include "UObject/ConstructorHelpers.h"
#include "Blueprint/UserWidget.h"
#include "OnlineSessionSettings.h"

#include "MainMenu.h"
#include "MenuWidget.h"

const static FName SESSION_NAME = TEXT("Game");
const static FName SERVER_NAME_SETTINGS_KEY = TEXT("ServerName");

/// <summary>
/// Game instance will be used to create the menus and pass in the button logic interface
/// </summary>
/// <param name="ObjectInitializer"></param>
UMultiplayerFPSGameInstance::UMultiplayerFPSGameInstance(const FObjectInitializer& ObjectInitializer)
{
	ConstructorHelpers::FClassFinder<UUserWidget> MenuBPClass(TEXT("/Game/UI/Menus/WBP_HomeMenu"));
	if (!ensure(MenuBPClass.Class != nullptr)) return;

	MenuClass = MenuBPClass.Class;

	ConstructorHelpers::FClassFinder<UUserWidget> InGameMenuBPClass(TEXT("/Game/UI/WBP_GameplayMenu"));
	if (!ensure(InGameMenuBPClass.Class != nullptr)) return;

	InGameMenuClass = InGameMenuBPClass.Class;
}

//All Clients have a GameInstance often used for persistent data
void UMultiplayerFPSGameInstance::Init()
{
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
			SessionInterface->OnJoinSessionCompleteDelegates.AddUObject(this, &UMultiplayerFPSGameInstance::OnJoinSessionComplete);
		}
	}
	else
		UE_LOG(LogTemp, Warning, TEXT("Subsystem is not null!"));

	if (GEngine != nullptr)
	{
		GEngine->OnNetworkFailure().AddUObject(this, &UMultiplayerFPSGameInstance::OnNetworkFailure);
	}
}

/// <summary>
/// Called from Level Blueprint (MainMenu)
/// </summary>
void UMultiplayerFPSGameInstance::LoadMenuWidget()
{
	if (!ensure(MenuClass != nullptr)) return;

	Menu = CreateWidget<UMainMenu>(this, MenuClass);
	if (!ensure(Menu != nullptr)) return;

	UE_LOG(LogTemp, Warning, TEXT("Menu Loaded"));

	Menu->Setup();

	//Passing in the interface to the base class menu
	Menu->SetMenuInterface(this);
}

/// <summary>
/// Called from Level Blueprint (Main)
/// </summary>
void UMultiplayerFPSGameInstance::InGameLoadMenu()
{
	if (!ensure(InGameMenuClass != nullptr)) return;

	UMenuWidget* GameMenu = CreateWidget<UMenuWidget>(this, InGameMenuClass);
	if (!ensure(GameMenu != nullptr)) return;

	GameMenu->Setup();
	GameMenu->SetMenuInterface(this);
}

/// <summary>
/// Interface
/// </summary>
void UMultiplayerFPSGameInstance::LoadMainMenu()
{
	APlayerController* PlayerController = GetFirstLocalPlayerController();
	if (!ensure(PlayerController != nullptr)) return;

	PlayerController->ClientTravel("/Game/UI/WBP_GameplayMenu", ETravelType::TRAVEL_Absolute);
}

/// <summary>
/// Interface
/// </summary>
/// <param name="ServerName"></param>
void UMultiplayerFPSGameInstance::Host(FString ServerName)
{
	DesiredServerName = ServerName;
	if (SessionInterface.IsValid())
	{
		auto ExistingSession = SessionInterface->GetNamedSession(SESSION_NAME);
		if (ExistingSession != nullptr)
		{
			//When a session ends and is no longer needed, the session is destroyed using the IOnlineSession::DestroySession() function. When the destruction opteration has completed, the OnDestroySessionComplete delegate is fired enabling you to perform cleanup operations.
			SessionInterface->DestroySession(SESSION_NAME);
		}
		else
		{
			CreateSession();
		}
	}
}

/// <summary>
/// Interface
/// </summary>
/// <param name="Index"></param>
void UMultiplayerFPSGameInstance::Join(uint32 Index)
{
	UE_LOG(LogTemp, Warning, TEXT("Join Session."));

	if (!SessionInterface.IsValid()) return;
	if (!SessionSearch.IsValid()) return;

	if (Menu != nullptr)
	{
		Menu->Teardown();
	}

	SessionInterface->JoinSession(0, SESSION_NAME, SessionSearch->SearchResults[Index]);
}

/// <summary>
/// Interface
/// </summary>
void UMultiplayerFPSGameInstance::RefreshServerList()
{
	SessionSearch = MakeShareable(new FOnlineSessionSearch());
	if (SessionSearch.IsValid())
	{
		//SessionSearch->bIsLanQuery = true;
		SessionSearch->MaxSearchResults = 10000;
		SessionSearch->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals);
		UE_LOG(LogTemp, Warning, TEXT("Starting Find Session"));
		SessionInterface->FindSessions(0, SessionSearch.ToSharedRef());
	}
}

void UMultiplayerFPSGameInstance::CreateSession()
{
	UE_LOG(LogTemp, Warning, TEXT("Create Session."));

	if (SessionInterface.IsValid()) {
		FOnlineSessionSettings SessionSettings;
		if (IOnlineSubsystem::Get()->GetSubsystemName() == "NULL")
		{
			SessionSettings.bIsLANMatch = true;
		}
		else
		{
			SessionSettings.bIsLANMatch = false;
			SessionSettings.bUseLobbiesIfAvailable = true;
		}

		SessionSettings.NumPublicConnections = 5;
		SessionSettings.bShouldAdvertise = true;
		SessionSettings.bUsesPresence = true;
		SessionSettings.Set(SERVER_NAME_SETTINGS_KEY, DesiredServerName, EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);

		//Sessions are created using IOnlineSession::CreateSession() which takes in a set of session settings that are used to configure the new session. Once the session is created, the OnCreateSessionComplete delegate is fired.
		SessionInterface->CreateSession(0, SESSION_NAME, SessionSettings);
	}
}

void UMultiplayerFPSGameInstance::StartSession()
{
	if (SessionInterface.IsValid())
	{
		SessionInterface->StartSession(SESSION_NAME);
	}
}

//Callbacks*********************************************************************************

void UMultiplayerFPSGameInstance::OnDestroySessionComplete(FName SessionName, bool Success)
{
	if (Success)
	{
		CreateSession();
	}
}

void UMultiplayerFPSGameInstance::OnFindSessionsComplete(bool Success)
{
	if (Success && SessionSearch.IsValid() && Menu != nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Finished Find Session. Number Found: %i"), SessionSearch->SearchResults.Num());

		TArray<FServerData> ServerNames;

		for (const FOnlineSessionSearchResult& SearchResult : SessionSearch->SearchResults)
		{
			UE_LOG(LogTemp, Warning, TEXT("Found session names: %s"), *SearchResult.GetSessionIdStr());
			FServerData Data;
			Data.MaxPlayers = SearchResult.Session.SessionSettings.NumPublicConnections;
			Data.CurrentPlayers = Data.MaxPlayers - SearchResult.Session.NumOpenPublicConnections;
			Data.HostUsername = SearchResult.Session.OwningUserName;
			FString ServerName;
			if (SearchResult.Session.SessionSettings.Get(SERVER_NAME_SETTINGS_KEY, ServerName))
			{
				Data.Name = ServerName;
			}
			else
			{
				Data.Name = "Could not find name.";
			}
			ServerNames.Add(Data);
		}

		Menu->SetServerList(ServerNames);
	}
}

void UMultiplayerFPSGameInstance::OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{
	if (!SessionInterface.IsValid()) return;

	FString Address;
	if (!SessionInterface->GetResolvedConnectString(SessionName, Address)) {
		UE_LOG(LogTemp, Warning, TEXT("Could not get connect string."));
		return;
	}

	UEngine* Engine = GetEngine();
	if (!ensure(Engine != nullptr)) return;

	Engine->AddOnScreenDebugMessage(0, 5, FColor::Green, FString::Printf(TEXT("Joining %s"), *Address));

	APlayerController* PlayerController = GetFirstLocalPlayerController();
	if (!ensure(PlayerController != nullptr)) return;

	UE_LOG(LogTemp, Warning, TEXT("Traveled"));
	PlayerController->ClientTravel(Address, ETravelType::TRAVEL_Absolute);
}

void UMultiplayerFPSGameInstance::OnNetworkFailure(UWorld* World, UNetDriver* NetDriver, ENetworkFailure::Type FailureType, const FString& ErrorString)
{
	LoadMainMenu();
}

void UMultiplayerFPSGameInstance::OnCreateSessionComplete(FName SessionName, bool Success)
{
	if (!Success)
	{
		UE_LOG(LogTemp, Warning, TEXT("Could not create session"));
		return;
	}

	if (Menu != nullptr)
	{
		Menu->Teardown();
	}

	UEngine* Engine = GetEngine();
	if (!ensure(Engine != nullptr)) return;

	Engine->AddOnScreenDebugMessage(0, 2, FColor::Green, TEXT("Hosting"));

	UWorld* World = GetWorld();
	if (!ensure(World != nullptr)) return;

	World->ServerTravel("/Game/Maps/Lobby?listen");
}