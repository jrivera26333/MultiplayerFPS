// Fill out your copyright notice in the Description page of Project Settings.


#include "HomeMenu.h"
#include "Components/Button.h"
#include "Components/WidgetSwitcher.h"

bool UHomeMenu::Initialize()
{
	bool Success = Super::Initialize();
	if (!Success) return false;

	HomeHost->OnClicked.AddDynamic(this, &UHomeMenu::HomeHostClicked);
	HomeJoin->OnClicked.AddDynamic(this, &UHomeMenu::HomeJoinClicked);
	HomeQuit->OnClicked.AddDynamic(this, &UHomeMenu::HomeQuitClicked);

	UE_LOG(LogTemp, Warning, TEXT("Home Init"));

	return true;
}

void UHomeMenu::HomeHostClicked()
{
	UE_LOG(LogTemp, Warning, TEXT("Home Host Pressed"));
	GetWidgetSwitcher()->SetActiveWidgetIndex(1);
}

void UHomeMenu::HomeJoinClicked()
{
	UE_LOG(LogTemp, Warning, TEXT("Home Join Pressed"));
	GetWidgetSwitcher()->SetActiveWidgetIndex(2);
}

void UHomeMenu::HomeQuitClicked()
{
	UE_LOG(LogTemp, Warning, TEXT("Home Quit Pressed"));
}