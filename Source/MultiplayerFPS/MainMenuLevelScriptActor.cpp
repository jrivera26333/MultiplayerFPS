// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenuLevelScriptActor.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"


void AMainMenuLevelScriptActor::BeginPlay()
{
	Super::BeginPlay();
}

void AMainMenuLevelScriptActor::CreateMainMenu()
{
	PlayerController = UGameplayStatics::GetPlayerController(this, 0);
	if (IsValid(MainMenuWidgetClass) && PlayerController != nullptr)
	{
		UUserWidget* MainMenuWidget = CreateWidget<UUserWidget>(PlayerController, MainMenuWidgetClass);

		if (MainMenuWidget)
		{
			MainMenuWidget->AddToViewport();
			FInputModeUIOnly InputModeUIOnly;
			PlayerController->SetInputMode(InputModeUIOnly);
			PlayerController->SetShowMouseCursor(true);
		}
	}
}