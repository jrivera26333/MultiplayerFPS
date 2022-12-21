// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "OnlineSubsystem.h"
#include "MultiplayerFPSGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class MULTIPLAYERFPS_API UMultiplayerFPSGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
protected:

	void Init() override;

private:

	void Host();
	void CreateSession();

	IOnlineSessionPtr SessionInterface;

	//Delegates
	void OnCreateSessionComplete(FName SessionName, bool Success);
	void OnDestroySessionComplete(FName SessionName, bool Success);
};
