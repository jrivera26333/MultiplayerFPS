// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSMachineGunSoldier.h"
#include "Animation/AnimSequence.h"
#include "GameFramework/CharacterMovementComponent.h"

const float MAX_WALK_SPEED = 600.f;
const float MAX_RUN_SPEED = 800.f;

AFPSMachineGunSoldier::AFPSMachineGunSoldier()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AFPSMachineGunSoldier::BeginPlay()
{
	Super::BeginPlay();

	CharacterMovementComp = GetCharacterMovement();

	if (HasAuthority())
	{
		if (CharacterMovementComp != nullptr)
		{
			CharacterMovementComp->SetIsReplicated(true);
			CharacterMovementComp->MaxWalkSpeed = MAX_WALK_SPEED;
		}
	}
}

void AFPSMachineGunSoldier::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction("HeldAbility", IE_Pressed, this, &AFPSMachineGunSoldier::Client_HeldSprint);
	PlayerInputComponent->BindAction("HeldAbility", IE_Released, this, &AFPSMachineGunSoldier::Client_ReleasedSprint);
}

void AFPSMachineGunSoldier::ServerSprintUpdate_Implementation()
{
	UE_LOG(LogTemp, Warning, TEXT("Sprinting Held Sprint Speed: %f"), GetVelocity().Size());

	if (CharacterMovementComp != nullptr)
		CharacterMovementComp->MaxWalkSpeed = MAX_RUN_SPEED;
}

void AFPSMachineGunSoldier::ServerWalkUpdate_Implementation()
{
	UE_LOG(LogTemp, Warning, TEXT("Sprinting Held Walk"));

	if (CharacterMovementComp != nullptr)
		CharacterMovementComp->MaxWalkSpeed = MAX_WALK_SPEED;
}

void AFPSMachineGunSoldier::Client_HeldSprint()
{
	//Client Update
	if (CharacterMovementComp != nullptr)
		CharacterMovementComp->MaxWalkSpeed = MAX_RUN_SPEED;

	ServerSprintUpdate();
}

void AFPSMachineGunSoldier::Client_ReleasedSprint()
{
	UE_LOG(LogTemp, Warning, TEXT("Sprinting Released"));

	//Client Update
	if (CharacterMovementComp != nullptr)
		CharacterMovementComp->MaxWalkSpeed = MAX_WALK_SPEED;

	ServerWalkUpdate();
}