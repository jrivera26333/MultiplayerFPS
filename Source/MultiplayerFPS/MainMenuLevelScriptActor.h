// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/LevelScriptActor.h"
#include "MainMenuLevelScriptActor.generated.h"

/**
 * 
 */
UCLASS()
class MULTIPLAYERFPS_API AMainMenuLevelScriptActor : public ALevelScriptActor
{
	GENERATED_BODY()
	
protected:

	virtual void BeginPlay();

	UPROPERTY(EditDefaultsOnly, Category = "Widget Menus")
	TSubclassOf<UUserWidget> MainMenuManagerWidgetClass;

	UPROPERTY(EditDefaultsOnly, Category = "Widget Menus")
	TSubclassOf<UUserWidget> HomeMenuWidgetClass;

	UPROPERTY(EditDefaultsOnly, Category = "Widget Menus")
	TSubclassOf<UUserWidget> HostMenuWidgetClass;

	UPROPERTY(EditDefaultsOnly, Category = "Widget Menus")
	TSubclassOf<UUserWidget> JoinMenuWidgetClass;

	UPROPERTY(EditDefaultsOnly, Category = "Widget Menus")
	TSubclassOf<UUserWidget> JoinRowWidgetClass;

private:

	UFUNCTION(BlueprintCallable)
	void CreateMainMenu();

	void CreateSubMenus();

	class UMainMenuManager* MainMenuWidget;

	class UHomeMenu* HomeMenuWidget;
	class UJoinMenu* JoinMenuWidget;
	class UHostMenu* HostMenuWidget;
	class UHostInfoRow* HostInfoRowWidget;

	APlayerController* PlayerController;
};