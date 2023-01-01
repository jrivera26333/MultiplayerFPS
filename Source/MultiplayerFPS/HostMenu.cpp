// Fill out your copyright notice in the Description page of Project Settings.


#include "HostMenu.h"
#include "Components/Button.h"
#include "Components/WidgetSwitcher.h"

bool UHostMenu::Initialize()
{
	bool Success = Super::Initialize();
	if (!Success) return false;

	HostEnter->OnClicked.AddDynamic(this, &UHostMenu::HostEnterClicked);
	HostCancel->OnClicked.AddDynamic(this, &UHostMenu::HostCancelClicked);

	return true;
}

void UHostMenu::HostEnterClicked()
{
	UE_LOG(LogTemp, Warning, TEXT("Host Enter Pressed"));
}

void UHostMenu::HostCancelClicked()
{
	UE_LOG(LogTemp, Warning, TEXT("Host Enter Pressed"));
	GetWidgetSwitcher()->SetActiveWidgetIndex(0);
}
