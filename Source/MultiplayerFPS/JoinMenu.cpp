// Fill out your copyright notice in the Description page of Project Settings.


#include "JoinMenu.h"
#include "Components/Button.h"
#include "Components/WidgetSwitcher.h"

bool UJoinMenu::Initialize()
{
	bool Success = Super::Initialize();
	if (!Success) return false;

	JoinEnter->OnClicked.AddDynamic(this, &UJoinMenu::JoinEnterClicked);
	JoinCancel->OnClicked.AddDynamic(this, &UJoinMenu::JoinCancelClicked);

	return true;
}

void UJoinMenu::JoinCancelClicked()
{
	UE_LOG(LogTemp, Warning, TEXT("Join Cancel Pressed"));
	GetWidgetSwitcher()->SetActiveWidgetIndex(0);
}

void UJoinMenu::JoinEnterClicked()
{
	UE_LOG(LogTemp, Warning, TEXT("Join Enter Pressed"));
}