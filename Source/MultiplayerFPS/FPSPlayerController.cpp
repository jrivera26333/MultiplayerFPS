#include "FPSPlayerController.h"
#include "FPSCharacter.h"
#include "PlayerMenu.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "FPSPlayerState.h"
#include "Net/UnrealNetwork.h"
#include "MultiplayerFPSGameModeBase.h"

void AFPSPlayerController::BeginPlay()
{
	Super::BeginPlay();

	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("Player Control Begin")));

	//This is a check for when we are the client and we are creating a PC on the server
	if (!IsLocalController() || PlayerMenuClass == nullptr)
	{
		return;		
	}

	PlayerMenu = CreateWidget<UPlayerMenu>(this, PlayerMenuClass);

	if (PlayerMenu != nullptr)
	{
		PlayerMenu->AddToViewport(0);
		AddAbilityPortraits();
		AddWeaponPortrait();
	}
}

void AFPSPlayerController::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AFPSPlayerController, PlayerNamesLoggedIn);
}

void AFPSPlayerController::UpdatePlayersUI()
{
	if (PlayerMenu != nullptr)
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("Player Menu Refreshed")));
		PlayerMenu->RefreshPlayerUI();
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("Player Menu Failed")));
	}
}

void AFPSPlayerController::ToggleScoreboard()
{
	if (PlayerMenu != nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Toggle Clicked!"));
		PlayerMenu->ToggleScoreboard();
	}
}

void AFPSPlayerController::OpenSettingsMenu()
{
	if (PlayerMenu != nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Settings Opened!"));
		PlayerMenu->OpenSettingsMenu();
	}
}

//Called from GameMode
void AFPSPlayerController::ServerGetPlayerNames_Implementation(const TArray<FString>& PlayerNamesRecieved)
{
	for (FString Name : PlayerNamesRecieved)
	{
		PlayerNamesLoggedIn.Add(Name);
	}

	if (GetLocalRole() == ROLE_Authority)
		UpdatePlayersUI();

	//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("Players Recieved: %s"), *FString::FromInt(PlayerNamesLoggedIn.Num())));
}

//Called from GameMode
void AFPSPlayerController::ClientUpdatePlayersUI_Implementation()
{
	UpdatePlayersUI();
}

//Called from GameMode
void AFPSPlayerController::ClientNotifyKill_Implementation(const FString& Name)
{
	if (PlayerMenu != nullptr)
	{
		PlayerMenu->NotifyKill(Name);
	}
}

//Called from GameMode
void AFPSPlayerController::ClientShowScoreboard_Implementation()
{
	if (PlayerMenu != nullptr)
	{
		PlayerMenu->SetScoreboardVisibility(true);
	}
}

void AFPSPlayerController::PostSeamlessTravel()
{
	Super::PostSeamlessTravel();
	ServerUpdatedGMPlayerHasLoaded();
}

void AFPSPlayerController::ServerUpdatedGMPlayerHasLoaded_Implementation()
{
	AMultiplayerFPSGameModeBase* FPSGameMode = (AMultiplayerFPSGameModeBase*)GetWorld()->GetAuthGameMode();

	if (FPSGameMode)
		FPSGameMode->AddToCurrentPlayersLoading(this);

	//if (FPSGameMode)
	//	FPSGameMode->SpawnPlayerTest(this);
}

void AFPSPlayerController::AddAbilityPortraits()
{
	if (Abilities.Num() > 0)
	{
		for (FAbilitySets Ability : Abilities)
		{
			//Blueprint Callable that is being called on the PlayerMenu_BP
			PlayerMenu->SetAbilityIcons(Ability.AbilityInput, Ability.AbilityTexture);
		}
	}
}

void AFPSPlayerController::AddWeaponPortrait()
{
	if (WeaponPortrait != nullptr)
	{
		PlayerMenu->SetWeaponPortrait(WeaponPortrait);
	}
}

void AFPSPlayerController::ResetPlayerReference()
{
	if (PlayerMenu != nullptr)
	{
		PlayerMenu->SetCharacterReference();
	}
}

void AFPSPlayerController::OwningClientPlaySound(USoundBase* Sound)
{
	if(IsLocalController())
		ClientPlaySound(Sound);
}