// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSSoldier.h"
#include "Net/UnrealNetwork.h"

//Reminder these values need to be replicated since we are firing the action itself on the Server
void AFPSSoldier::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(AFPSSoldier, MissleCoolDown, COND_OwnerOnly);
	DOREPLIFETIME_CONDITION(AFPSSoldier, AmountOfDamage, COND_OwnerOnly);
}

void AFPSSoldier::SecondaryAbility()
{
	if (GetWorldTimerManager().GetTimerRemaining(FireTimer) > 0.0f)
		return;

	FireRocket();
	GetWorldTimerManager().SetTimer(FireTimer, SecondaryAbilityFireRate, false);
}

void AFPSSoldier::FireRocket()
{
	UE_LOG(LogTemp, Warning, TEXT("Soldier Secondary Ability Fired!"));
}