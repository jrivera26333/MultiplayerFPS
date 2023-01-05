// Fill out your copyright notice in the Description page of Project Settings.


#include "HomeMenu.h"
#include "Components/Button.h"
#include "Components/WidgetSwitcher.h"
#include "UObject/ConstructorHelpers.h"
#include "HostInfoRow.h"
#include "Blueprint/UserWidget.h"

UHomeMenu::UHomeMenu(const FObjectInitializer& ObjectInitializer)
{
	ConstructorHelpers::FClassFinder<UUserWidget> ServerRowBPClass(TEXT("/Game/UI/Menus/UI_Row"));
	if (!ensure(ServerRowBPClass.Class != nullptr)) return;

	ServerRowClass = ServerRowBPClass.Class;
}

bool UHomeMenu::Initialize()
{
	bool Success = Super::Initialize();
	if (!Success) return false;

	HomeHost->OnClicked.AddDynamic(this, &UHomeMenu::HomeHostClicked);
	HomeJoin->OnClicked.AddDynamic(this, &UHomeMenu::HomeJoinClicked);
	HomeQuit->OnClicked.AddDynamic(this, &UHomeMenu::HomeQuitClicked);

	HostEnter->OnClicked.AddDynamic(this, &UHomeMenu::HomeHostClicked);
	HostCancel->OnClicked.AddDynamic(this, &UHomeMenu::HomeJoinClicked);
	JoinEnter->OnClicked.AddDynamic(this, &UHomeMenu::HomeQuitClicked);
	JoinCancel->OnClicked.AddDynamic(this, &UHomeMenu::HomeQuitClicked);

	return true;
}

void UHomeMenu::HomeHostClicked()
{
	UE_LOG(LogTemp, Warning, TEXT("Home Host Pressed"));

	if(GetWidgetSwitcher())
		GetWidgetSwitcher()->SetActiveWidgetIndex(1);
}

void UHomeMenu::HomeJoinClicked()
{
	UE_LOG(LogTemp, Warning, TEXT("Home Join Pressed"));

	if (GetWidgetSwitcher())
		GetWidgetSwitcher()->SetActiveWidgetIndex(2);
}

void UHomeMenu::HomeQuitClicked()
{
	UE_LOG(LogTemp, Warning, TEXT("Home Quit Pressed"));
}

void UHomeMenu::HostEnterClicked()
{
}

void UHomeMenu::HomeCancelClicked()
{
}

void UHomeMenu::JoinEnterClicked()
{
}

void UHomeMenu::JoinCancelClicked()
{
}
