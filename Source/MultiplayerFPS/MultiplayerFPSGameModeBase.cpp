#include "MultiplayerFPSGameModeBase.h"
#include "FPSPlayerState.h"
#include "FPSGameState.h"
#include "FPSCharacter.h"
#include "FPSPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "Engine/Public/TimerManager.h"
#include "FPSPlayerStart.h"
#include "TimerManager.h"
#include "MultiplayerFPSGameInstance.h"
#include "Math/UnrealMathUtility.h"
#include "Engine/Engine.h"

//Game mode exists only on the server, and in our case Listen Server
AMultiplayerFPSGameModeBase::AMultiplayerFPSGameModeBase()
{
	UE_LOG(LogTemp, Warning, TEXT("Game Mode!"));
	//DefaultPawnClass = AFPSCharacter::StaticClass();

	DefaultPawnClass = nullptr;
	PlayerControllerClass = AFPSPlayerController::StaticClass();
	PlayerStateClass = AFPSPlayerState::StaticClass();
	GameStateClass = AFPSGameState::StaticClass();

	CurrentGameState = GetGameState<AFPSGameState>();
	FindPlayerStarts();

	PrimaryActorTick.bCanEverTick = true;
}

//Called after a successful login
void AMultiplayerFPSGameModeBase::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("Post Login")));

	AFPSPlayerController* LoggedPlayerController = Cast<AFPSPlayerController>(NewPlayer);

	if (LoggedPlayerController)
	{
		if (!PlayersLoggedIn.Contains(LoggedPlayerController))
		{
			PlayersLoggedIn.Add(LoggedPlayerController);
		}
	}

	++NumberOfPlayers;
	if (NumberOfPlayers >= 2)
	{
		GetWorldTimerManager().SetTimer(GameStartTimer, this, &AMultiplayerFPSGameModeBase::StartGame, 5);
	}
}

void AMultiplayerFPSGameModeBase::StartGame()
{
	auto GameInstance = Cast<UMultiplayerFPSGameInstance>(GetGameInstance());
	UE_LOG(LogTemp, Warning, TEXT("Starting Game"));

	if (GameInstance == nullptr) return;

	GameInstance->StartSession();

	UWorld* World = GetWorld();
	if (!ensure(World != nullptr)) return;

	bUseSeamlessTravel = true;

	World->ServerTravel("/Game/Maps/Main?listen");
	HasStartedTraveling = true;
}

void AMultiplayerFPSGameModeBase::SpawnPlayerTest()
{

}

void AMultiplayerFPSGameModeBase::SpawnInitialPlayer(AFPSPlayerController* PlayerController)
{
	GEngine->AddOnScreenDebugMessage(-1, 60.f, FColor::Yellow, FString::Printf(TEXT("Player Controller: %s"), *PlayerController->GetName()));
	DefaultPawnClass = AFPSCharacter::StaticClass();
	RestartPlayer(PlayerController);
}

void AMultiplayerFPSGameModeBase::FindPlayerStarts()
{
	TSubclassOf<AFPSPlayerStart> PlayerStart = AFPSPlayerStart::StaticClass();
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), PlayerStart, PlayerStarts);
}

bool AMultiplayerFPSGameModeBase::ShouldSpawnAtStartSpot(AController* Player)
{
	// We want to spawn at a random location, not always in the same starting spot
	return false;
}

/// <summary>
/// The reason for setting the KillLimit on the GameState is because it can be easily accessed to Clients. In our case the widget.
/// </summary>
void AMultiplayerFPSGameModeBase::HandleMatchHasStarted()
{
	Super::HandleMatchHasStarted();

	UE_LOG(LogTemp, Warning, TEXT("Match Started"));
	// Tell the kill limit to the game state

	AFPSGameState* FPSGameState = Cast<AFPSGameState>(GameState);

	if (FPSGameState != nullptr)
	{
		FPSGameState->SetKillLimit(KillLimit);
	}
}
/// <summary>
/// Called when ReadyToEndMatch returns true
/// </summary>
void AMultiplayerFPSGameModeBase::HandleMatchHasEnded()
{
	Super::HandleMatchHasEnded();

	UE_LOG(LogTemp, Warning, TEXT("Match Ended"));
	TArray<AActor*> PlayerControllers;

	UGameplayStatics::GetAllActorsOfClass(this, AFPSPlayerController::StaticClass(), PlayerControllers);

	for (AActor* PlayerController : PlayerControllers)
	{
		AFPSPlayerController* FPSPlayerController = Cast<AFPSPlayerController>(PlayerController);

		if (FPSPlayerController == nullptr)
			continue;

		APawn* Pawn = FPSPlayerController->GetPawn();

		if (Pawn != nullptr)
		{
			Pawn->Destroy();
		}

		FPSPlayerController->ClientShowScoreboard();
	}

	FTimerHandle TimerHandle;
	GetWorldTimerManager().SetTimer(TimerHandle, this, &AMultiplayerFPSGameModeBase::RestartMap, 5.0f);
	UE_LOG(LogTemp, Warning, TEXT("Restart Map Called!")); 
}

void AMultiplayerFPSGameModeBase::Tick(float dt)
{
	Super::Tick(dt);

	if (NumTravellingPlayers <= 0 && HasStartedTraveling)
	{
		for (auto PlayerController : PlayersLoggedIn)
		{
			SpawnInitialPlayer(PlayerController);
			GEngine->AddOnScreenDebugMessage(-1, 60.f, FColor::Yellow, FString::Printf(TEXT("Spawned Player")));
		}

		HasStartedTraveling = false;
	}
}

void AMultiplayerFPSGameModeBase::RestartMap()
{
	UE_LOG(LogTemp, Warning, TEXT("Map Name: %s"), *GetWorld()->GetName());
	//GetWorld()->ServerTravel(GetWorld()->GetName(), false, false);

	TArray<AActor*> PlayerControllers;
	UGameplayStatics::GetAllActorsOfClass(this, AFPSPlayerController::StaticClass(), PlayerControllers);

	for (AActor* PlayerController : PlayerControllers)
	{
		AFPSPlayerController* FPSPlayerController = Cast<AFPSPlayerController>(PlayerController);
		FPSPlayerController->ClientTravel("/Game/Maps/MainMenu", ETravelType::TRAVEL_Absolute);
	}
}

//Constantly being checked in the GameMode
bool AMultiplayerFPSGameModeBase::ReadyToEndMatch_Implementation()
{
	return HasWinner();
}

bool AMultiplayerFPSGameModeBase::HasWinner() const
{
	// Go through the player states and check if any of the players have reached the kill limit

	for (APlayerState* PlayerState : GameState->PlayerArray)
	{
		AFPSPlayerState* FPSPlayerState = Cast<AFPSPlayerState>(PlayerState);

		if (FPSPlayerState != nullptr && FPSPlayerState->GetKills() == KillLimit)
		{
			return true;
		}
	}

	return false;
}

void AMultiplayerFPSGameModeBase::OnKill(AController* KillerController, AController* VictimController)
{
	if (!IsMatchInProgress())
	{
		return;
	}

	// Add kill to the killer

	if (KillerController != nullptr && KillerController != VictimController)
	{
		AFPSPlayerState* KillerPlayerState = Cast<AFPSPlayerState>(KillerController->PlayerState);

		if (KillerPlayerState != nullptr)
		{
			//Stats are can be added as a Player State. However its not unheard of assigning them in the GameState as well for all Widgets to easily handle
			KillerPlayerState->AddKill();
		}

		// Show the kill on the killer's HUD

		AFPSPlayerController* KillerFPSController = Cast<AFPSPlayerController>(KillerController);

		if (KillerFPSController != nullptr && VictimController != nullptr && VictimController->PlayerState != nullptr)
		{
			KillerFPSController->ClientNotifyKill(VictimController->PlayerState->GetPlayerName());
		}
	}

	// Add death to the victim

	if (VictimController != nullptr)
	{
		AFPSPlayerState* VictimPlayerState = Cast<AFPSPlayerState>(VictimController->PlayerState);

		if (VictimPlayerState != nullptr)
		{
			VictimPlayerState->AddDeath();
		}

		APawn* Pawn = VictimController->GetPawn();

		if (Pawn != nullptr)
		{
			Pawn->Destroy();
		}

		if (!HasWinner())
		{
			int32 RandomIndex = FMath::RandRange(0, PlayerStarts.Num() - 1);
			RestartPlayerAtPlayerStart(VictimController, PlayerStarts[RandomIndex]);
		}
	}

	
}

AActor* AMultiplayerFPSGameModeBase::InitialSpawn(AFPSPlayerController* LoggedInPlayer)
{
	UE_LOG(LogTemp, Warning, TEXT("Spawned!"));

	//If player has not been spawned
	if (!LoggedInPlayer->GetHasSpawnBeenSet())
	{
		LoggedInPlayer->SetPlayerNumber(SpawnCounter);
		LoggedInPlayer->SetHasSpawnBeenSet(true);
		SpawnCounter++;
	}

	//Geting the Player Spawn Number and using that for the Spawn Array Index
	if (PlayerStarts.IsValidIndex(LoggedInPlayer->GetPlayerNumber()))
	{
		return PlayerStarts[LoggedInPlayer->GetPlayerNumber()];
	}

	return PlayerStarts[PlayerStarts.Num() - 1];
}

AActor* AMultiplayerFPSGameModeBase::ChoosePlayerStart_Implementation(AController* Player)
{
	AFPSPlayerController* LoggedInPlayer = Cast<AFPSPlayerController>(Player);
	return InitialSpawn(LoggedInPlayer);
}