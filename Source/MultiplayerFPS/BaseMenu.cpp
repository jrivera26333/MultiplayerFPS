// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseMenu.h"
#include "Components/WidgetSwitcher.h"

bool UBaseMenu::Initialize()
{
	bool Success = Super::Initialize();
	if (!Success) return false;

	SetFPSGameInstance();
	return true;
}