#include "FPSPlayerController.h"
#include "FPSCharacter.h"
#include "PlayerMenu.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "FPSPlayerState.h"
#include "Net/UnrealNetwork.h"
#include "MultiplayerFPSGameModeBase.h"
#include "FPSGameState.h"

void AFPSPlayerController::PostSeamlessTravel()
{
	Super::PostSeamlessTravel();

	//Updating GM we have loaded into the level
	AMultiplayerFPSGameModeBase* FPSGameMode = (AMultiplayerFPSGameModeBase*)GetWorld()->GetAuthGameMode();

	if (FPSGameMode)
		FPSGameMode->AddToCurrentPlayersLoading(this);
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
void AFPSPlayerController::CreatePlayerMenuWidget_Implementation()
{
	//This is a check for when we are the client and we are creating a PC on the server
	if (!IsLocalController() || PlayerMenuClass == nullptr) return;

	PlayerMenu = CreateWidget<UPlayerMenu>(this, PlayerMenuClass);

	if (PlayerMenu != nullptr)
	{
		PlayerMenu->AddToViewport(0);
		AddAbilityPortraits();
		AddWeaponPortrait();
	}
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

//Called from GameMode. TODO: Refactor and duplicate 
void AFPSPlayerController::ClientUpdatePlayersUI_Implementation(const TArray<APlayerState*>& PlayerStateArray)
{
	if (PlayerMenu != nullptr)
	{
		PlayerMenu->SetupPlayersUI();
	}
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

void AFPSPlayerController::OwningClientPlaySound(USoundBase* Sound)
{
	if(IsLocalController())
		ClientPlaySound(Sound);
}

void AFPSPlayerController::RefreshKills()
{
	if (PlayerMenu != nullptr)
	{
		PlayerMenu->RefreshKillsUI();
	}
}