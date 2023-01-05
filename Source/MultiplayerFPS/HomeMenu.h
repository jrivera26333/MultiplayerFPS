// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BaseMenu.h"
#include "HomeMenu.generated.h"

/**
 * 
 */
UCLASS()
class MULTIPLAYERFPS_API UHomeMenu : public UBaseMenu
{
	GENERATED_BODY()

protected:

	virtual bool Initialize() override;

	//Buttons

	UPROPERTY(meta = (BindWidget))
	class UButton* HomeHost;

	UPROPERTY(meta = (BindWidget))
	class UButton* HomeJoin;

	UPROPERTY(meta = (BindWidget))
	class UButton* HomeQuit;

	UPROPERTY(meta = (BindWidget))
	class UButton* HostEnter;

	UPROPERTY(meta = (BindWidget))
	class UButton* HostCancel;

	UPROPERTY(meta = (BindWidget))
	class UButton* JoinEnter;

	UPROPERTY(meta = (BindWidget))
	class UButton* JoinCancel;

	//Menus

	UPROPERTY(meta = (BindWidget))
	class UWidget* UIHomeMenu;

	UPROPERTY(meta = (BindWidget))
	class UWidget* JoinMenu;

	UPROPERTY(meta = (BindWidget))
	class UWidget* HostMenu;

	//Switcher

	UPROPERTY(meta = (BindWidget))
	class UWidgetSwitcher* WidgetSwitcherMenu;

	//Misc

	UPROPERTY(meta = (BindWidget))
	class UEditableTextBox* ServerHostName;

	UPROPERTY(meta = (BindWidget))
	class UPanelWidget* RollScrollBox;

private:

	TSubclassOf<class UUserWidget> ServerRowClass;

	UFUNCTION()
	void HomeHostClicked();

	UFUNCTION()
	void HomeJoinClicked();

	UFUNCTION()
	void HomeQuitClicked();

	UFUNCTION()
	void HostEnterClicked();

	UFUNCTION()
	void HomeCancelClicked();

	UFUNCTION()
	void JoinEnterClicked();

	UFUNCTION()
	void JoinCancelClicked();

public:
	UHomeMenu(const FObjectInitializer& ObjectInitializer);
};
