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

	UPROPERTY(EditDefaultsOnly, Category = "Widget Class")
	TSubclassOf<UUserWidget> MainMenuWidgetClass;

private:

	UFUNCTION(BlueprintCallable)
	void CreateMainMenu();

	APlayerController* PlayerController;
};