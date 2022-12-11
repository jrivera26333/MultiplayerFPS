// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSMachineGunSoldier.h"
#include "Net/UnrealNetwork.h"
#include "Animation/AnimSequence.h"

//Reminder these values need to be replicated since we are firing the action itself on the Server
void AFPSMachineGunSoldier::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(AFPSMachineGunSoldier, MissleCoolDown, COND_OwnerOnly);
	DOREPLIFETIME_CONDITION(AFPSMachineGunSoldier, AmountOfDamage, COND_OwnerOnly);
}

void AFPSMachineGunSoldier::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("HeldAbility", this, &AFPSMachineGunSoldier::Sprint);
}

void AFPSMachineGunSoldier::Sprint(float Value)
{
	GetWorldTimerManager().SetTimer(FireTimer, SecondaryAbilityFireRate, false);
	UE_LOG(LogTemp, Warning, TEXT("Soldier Secondary Ability Fired!"));
	GetMesh()->PlayAnimation(SprintAnimation, true);
}
