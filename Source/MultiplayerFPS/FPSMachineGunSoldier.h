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

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	UAnimSequence* SprintAnimation;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void BeginPlay() override;

	AFPSMachineGunSoldier();

private:

	FTimerHandle FireTimer;

	void Sprint();

	bool IsSprinting = false;

	UCharacterMovementComponent* CharacterMovementComp;

	UFUNCTION(Server, Unreliable)
	void ServerSprintUpdate();

	UFUNCTION(Server, Unreliable)
	void ServerWalkUpdate();

	void Client_HeldSprint();
	void Client_ReleasedSprint();
};
