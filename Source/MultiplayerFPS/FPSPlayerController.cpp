#include "FPSPlayerController.h"
#include "FPSCharacter.h"
#include "PlayerMenu.h"
#include "Blueprint/UserWidget.h"

void AFPSPlayerController::BeginPlay()
{
	Super::BeginPlay();

	//Allows only client (listen server included to create UI)
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