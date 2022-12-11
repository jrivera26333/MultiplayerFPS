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
	virtual void Tick(float DeltaSeconds) override;

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

	//Replicating because we are firing our gun on the server which then needs to update the bullets on the client. Remember the call is only happening on the Server
	//UPROPERTY(Replicated)
	//bool IsSprinting = false;;

	void Client_HeldSprint();
	void Client_ReleasedSprint();
};
