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

	UPROPERTY(meta = (BindWidget))
	class UButton* HomeHost;

	UPROPERTY(meta = (BindWidget))
	class UButton* HomeJoin;

	UPROPERTY(meta = (BindWidget))
	class UButton* HomeQuit;

private:

	UFUNCTION()
	void HomeHostClicked();

	UFUNCTION()
	void HomeJoinClicked();

	UFUNCTION()
	void HomeQuitClicked();
};
