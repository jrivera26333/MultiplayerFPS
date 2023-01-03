// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainMenuManager.generated.h"

/**
 * 
 */
UCLASS()
class MULTIPLAYERFPS_API UMainMenuManager : public UUserWidget
{
	GENERATED_BODY()

protected:

	UPROPERTY(meta = (BindWidget))
	class UWidgetSwitcher* WidgetSwitcherMenu;

public:
	class UWidgetSwitcher* GetMenuWidgetSwitcher() const {return WidgetSwitcherMenu;}
};
