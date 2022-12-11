// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FPSCharacter.h"
#include "FPSMachineGunSoldier.generated.h"

/**
 * 
 */
UCLASS()
class MULTIPLAYERFPS_API AFPSMachineGunSoldier : public AFPSCharacter
{
	GENERATED_BODY()

protected:

	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Soldier")
	float MissleCoolDown = 5.f;

	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Soldier")
	float AmountOfDamage = 5.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Abilities")
	float SecondaryAbilityFireRate = 3.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	UAnimSequence* SprintAnimation;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:

	FTimerHandle FireTimer;
	void Sprint(float Value);
};
