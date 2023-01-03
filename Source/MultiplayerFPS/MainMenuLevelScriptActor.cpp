// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenuLevelScriptActor.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Components/WidgetSwitcher.h"
#include "MainMenuManager.h"
#include "HomeMenu.h"
#include "JoinMenu.h"
#include "HostMenu.h"


void AMainMenuLevelScriptActor::BeginPlay()
{
	Super::BeginPlay();
}

void AMainMenuLevelScriptActor::CreateMainMenu()
{
	PlayerController = UGameplayStatics::GetPlayerController(this, 0);
	if (IsValid(MainMenuManagerWidgetClass) && PlayerController != nullptr)
	{
		MainMenuWidget = CreateWidget<UMainMenuManager>(PlayerController, MainMenuManagerWidgetClass);

		if (MainMenuWidget)
		{
			UE_LOG(LogTemp, Warning, TEXT("Added to Viewport"));
			MainMenuWidget->AddToViewport();
			FInputModeUIOnly InputModeUIOnly;
			PlayerController->SetInputMode(InputModeUIOnly);
			PlayerController->SetShowMouseCursor(true);
			CreateSubMenus();
		}
		else
			UE_LOG(LogTemp, Warning, TEXT("Not added to Viewport"));
	}
}

void AMainMenuLevelScriptActor::CreateSubMenus()
{
	if (MainMenuWidget)
	{
		auto MenuSwitcher = MainMenuWidget->GetMenuWidgetSwitcher();

		HomeMenuWidget = CreateWidget<UHomeMenu>(PlayerController, HomeMenuWidgetClass);
		HostMenuWidget = CreateWidget<UHostMenu>(PlayerController, HostMenuWidgetClass);
		JoinMenuWidget = CreateWidget<UJoinMenu>(PlayerController, JoinMenuWidgetClass);

		MenuSwitcher->AddChild(HomeMenuWidget);
		MenuSwitcher->AddChild(HostMenuWidget);
;		MenuSwitcher->AddChild(JoinMenuWidget);

		HomeMenuWidget->SetWidgetSwitcher(MainMenuWidget->GetMenuWidgetSwitcher());
		HostMenuWidget->SetWidgetSwitcher(MainMenuWidget->GetMenuWidgetSwitcher());
		JoinMenuWidget->SetWidgetSwitcher(MainMenuWidget->GetMenuWidgetSwitcher());

		JoinMenuWidget->SetRowWidget(JoinRowWidgetClass);
	}
}