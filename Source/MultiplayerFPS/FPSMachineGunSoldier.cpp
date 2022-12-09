// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSMachineGunSoldier.h"
#include "Net/UnrealNetwork.h"

//Reminder these values need to be replicated since we are firing the action itself on the Server
void AFPSMachineGunSoldier::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(AFPSMachineGunSoldier, MissleCoolDown, COND_OwnerOnly);
	DOREPLIFETIME_CONDITION(AFPSMachineGunSoldier, AmountOfDamage, COND_OwnerOnly);
}

void AFPSMachineGunSoldier::SecondaryAbility()
{
	if (GetWorldTimerManager().GetTimerRemaining(FireTimer) > 0.0f)
		return;

	FireRocket();
	GetWorldTimerManager().SetTimer(FireTimer, SecondaryAbilityFireRate, false);
}

void AFPSMachineGunSoldier::FireRocket()
{
	UE_LOG(LogTemp, Warning, TEXT("Soldier Secondary Ability Fired!"));
}
