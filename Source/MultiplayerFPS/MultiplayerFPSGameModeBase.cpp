#include "MultiplayerFPSGameModeBase.h"
#include "FPSPlayerState.h"
#include "FPSGameState.h"
#include "FPSMachineGunSoldier.h"
#include "FPSPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "Engine/Public/TimerManager.h"
#include "FPSPlayerStart.h"
#include "TimerManager.h"
#include "MultiplayerFPSGameInstance.h"
#include "Math/UnrealMathUtility.h"
#include "Engine/Engine.h"

const uint32 MAX_PLAYERS_IN_LOBBY = 2;

//Game mode exists only on the server, and in our case Listen Server
AMultiplayerFPSGameModeBase::AMultiplayerFPSGameModeBase()
{
	//DefaultPawnClass = AFPSCharacter::StaticClass();

	DefaultPawnClass = nullptr;
	PlayerControllerClass = AFPSPlayerController::StaticClass();
	PlayerStateClass = AFPSPlayerState::StaticClass();
	GameStateClass = AFPSGameState::StaticClass();

	CurrentGameState = GetGameState<AFPSGameState>();	
	PrimaryActorTick.bCanEverTick = true;
}

void AMultiplayerFPSGameModeBase::Tick(float dt)
{
	Super::Tick(dt);

	if (CurrentPlayersLoaded >= MAX_PLAYERS_IN_LOBBY && !HasSpawnedPlayers)
	{
		AFPSGameState* FPSGameState = GetWorld()->GetGameState<AFPSGameState>();

		for (auto PlayerController : PlayersLoggedIn)
		{
			SpawnPlayerTest(PlayerController);
		}

		for (auto PlayerController : PlayersLoggedIn)
		{
			PlayerController->ClientUpdatePlayersUI(FPSGameState->PlayerArray);
		}

		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("Spawned Players"), *FString::FromInt(NumTravellingPlayers)));
		HasSpawnedPlayers = true;
	}
}

void AMultiplayerFPSGameModeBase::AddToCurrentPlayersLoading(class AFPSPlayerController* AddedPlayerController)
{
	if (AddedPlayerController)
	{
		if (!PlayersLoggedIn.Contains(AddedPlayerController))
		{
			PlayersLoggedIn.Add(AddedPlayerController);
			++CurrentPlayersLoaded;
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("Added")));
		}
	}
}

void AMultiplayerFPSGameModeBase::PostSeamlessTravel()
{
	Super::PostSeamlessTravel();
	FindPlayerStarts();
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("Finding PlayerStarts")));
}

void AMultiplayerFPSGameModeBase::SpawnPlayerTest(AFPSPlayerController* PlayerController)
{
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("Spawned")));
	if (PlayerController == nullptr) return;

	APawn* PlayerFPSPawn = PlayerController->GetPawn();

	if (PlayerFPSPawn)
		PlayerFPSPawn->Destroy();

	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	auto Character = GetWorld()->SpawnActor<AFPSMachineGunSoldier>(MachineGunSoldierClass, PlayerStarts[FMath::RandRange(0, PlayerStarts.Num())]->GetTransform(), SpawnParams);
	PlayerController->Possess(Character);

	//The transition level UI lingers so when we spawn our Player Character we will transition into the GamePlay UI. BeginPlay on FPSPlayerController is to soon!
	PlayerController->CreatePlayerMenuWidget();
}

void AMultiplayerFPSGameModeBase::FindPlayerStarts()
{
	TSubclassOf<APlayerStart> PlayerStartClass = APlayerStart::StaticClass();
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), PlayerStartClass, PlayerStarts);

	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("PlayerStarts Found: %s"),*FString::FromInt(PlayerStarts.Num())));
}

bool AMultiplayerFPSGameModeBase::ShouldSpawnAtStartSpot(AController* Player)
{
	Super::ShouldSpawnAtStartSpot(Player);

	// We want to spawn at a random location, not always in the same starting spot
	return false;
}

/// <summary>
/// The reason for setting the KillLimit on the GameState is because it can be easily accessed to Clients. In our case the widget.
/// </summary>
void AMultiplayerFPSGameModeBase::HandleMatchHasStarted()
{
	Super::HandleMatchHasStarted();

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
}

void AMultiplayerFPSGameModeBase::RestartMap()
{
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