// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MultiplayerFPSGameInstance.h"
#include "BaseMenu.generated.h"

/**
 * 
 */
UCLASS()
class MULTIPLAYERFPS_API UBaseMenu : public UUserWidget
{
	GENERATED_BODY()

protected:

	virtual bool Initialize() override;

	class UWidgetSwitcher* WidgetSwitcher;

	UWidgetSwitcher* GetWidgetSwitcher() const { return WidgetSwitcher; }

	UMultiplayerFPSGameInstance* FPSGameInstance;
	UMultiplayerFPSGameInstance* GetFPSGameInstance() const {return FPSGameInstance;}
	void SetFPSGameInstance() { FPSGameInstance = Cast<UMultiplayerFPSGameInstance>(GetGameInstance()); }

public:
	void SetWidgetSwitcher(UWidgetSwitcher* MainMenuWidget) { WidgetSwitcher = MainMenuWidget; }
};
