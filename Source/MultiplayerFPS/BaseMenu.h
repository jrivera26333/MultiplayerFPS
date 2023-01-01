// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BaseMenu.generated.h"

/**
 * 
 */
UCLASS()
class MULTIPLAYERFPS_API UBaseMenu : public UUserWidget
{
	GENERATED_BODY()

protected:
	class UWidgetSwitcher* WidgetSwitcher;

	UFUNCTION(BlueprintCallable)
	void SetWidgetSwitcher(UWidgetSwitcher* MainMenuWidget) { WidgetSwitcher = MainMenuWidget; }

	UWidgetSwitcher* GetWidgetSwitcher() const { return WidgetSwitcher; }
};
