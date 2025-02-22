// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerStart.h"
#include "FPSPlayerStart.generated.h"

/**
 * 
 */
UCLASS()
class MULTIPLAYERFPS_API AFPSPlayerStart : public APlayerStart
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	bool IsStartingSpawnPos;

	bool HasBeenTaken;

	bool GetIsStartingSpawnPos() { return IsStartingSpawnPos; };

	bool GetHasBeenTakenSpawnPos() { return HasBeenTaken; };
	void SetHasBeenTakenSpawnPos(bool IsTaken) { HasBeenTaken = IsTaken; };
};
