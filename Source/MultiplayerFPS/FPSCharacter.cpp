#include "FPSCharacter.h"
#include "Net/UnrealNetwork.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Components/CapsuleComponent.h"
#include "MultiplayerFPSGameModeBase.h"
#include "FPSPlayerController.h"
#include "Weapon.h"
#include "GameFramework/SpringArmComponent.h"
#include "FPSPlayerController.h"
#include "MultiplayerFPSGameModeBase.h"

AFPSCharacter::AFPSCharacter()
{
	CameraComponent = CreateDefaultSubobject<UCameraComponent>("Camera");
	CameraComponent->bUsePawnControlRotation = true;
	CameraComponent->SetupAttachment(GetMesh(), "Camera");

	// Set the max walk speed to 800 and the jump z velocity to 600

	GetCharacterMovement()->MaxWalkSpeed = 800.0f;
	GetCharacterMovement()->JumpZVelocity = 600.0f;

	// This character doesn’t need to tick

	PrimaryActorTick.bCanEverTick = false;
}

//Each Client gets a PlayerController on the Server which then replicates to its client. That's why we our replicating our weapon and setting a majority of our values there. Plus we don't want the client to be able to set values. No need in running it on both the Client and the Server
void AFPSCharacter::BeginPlay()
{
	Super::BeginPlay();

	UGameplayStatics::PlaySound2D(GetWorld(), SpawnSound);

	if (!HasAuthority()) return;

	//SetHealth(MaxHealth);
	SetHealth(MaxHealth);
	AddWeapon(WeaponClass);

	GameMode = Cast<AMultiplayerFPSGameModeBase>(GetWorld()->GetAuthGameMode());
}

void AFPSCharacter::Restart()
{
	Super::Restart();

	if (IsLocallyControlled())
	{
		LocalFPSController = Cast<AFPSPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));

		if (LocalFPSController)
		{
			UE_LOG(LogTemp, Warning, TEXT("Possessed!"));
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Not Possessed!"));
		}
	}
}

void AFPSCharacter::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	if (Weapon != nullptr)
	{
		Weapon->Destroy();
	}
}

void AFPSCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Bind the action mappings

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AFPSCharacter::OnPressedJump);
	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &AFPSCharacter::OnPressedFire);
	PlayerInputComponent->BindAction("Fire", IE_Released, this, &AFPSCharacter::OnReleasedFire);
	PlayerInputComponent->BindAction("Reload", IE_Pressed, this, &AFPSCharacter::OnPressedReload);
	PlayerInputComponent->BindAction("Settings", IE_Pressed, this, &AFPSCharacter::OnPressedSettings);

	//PlayerInputComponent->BindAction("Scoreboard", IE_Pressed, this, &AFPSCharacter::OnPressedScoreboard);

	// Bind the axis mappings

	PlayerInputComponent->BindAxis("MoveForward", this, &AFPSCharacter::OnAxisMoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AFPSCharacter::OnAxisMoveRight);
	PlayerInputComponent->BindAxis("LookUp", this, &AFPSCharacter::OnAxisLookUp);
	PlayerInputComponent->BindAxis("Turn", this, &AFPSCharacter::OnAxisTurn);

	UE_LOG(LogTemp, Warning, TEXT("Binded!"));
}

void AFPSCharacter::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(AFPSCharacter, Health, COND_OwnerOnly);
	DOREPLIFETIME_CONDITION(AFPSCharacter, Weapon, COND_OwnerOnly);
}

void AFPSCharacter::OnPressedJump()
{
	Jump();
}

void AFPSCharacter::OnPressedReload()
{
	if (Weapon != nullptr)
	{
		Weapon->Reload();
	}
}

void AFPSCharacter::OnPressedFire()
{
	if (Weapon != nullptr)
	{
		Weapon->OnPressedFire();
	}
}

void AFPSCharacter::OnReleasedFire()
{
	if (Weapon != nullptr)
	{
		Weapon->OnReleasedFire();
	}
}

/// <summary>
/// TODO: Implement Later
/// </summary>
void AFPSCharacter::OnPressedScoreboard()
{
	UE_LOG(LogTemp, Warning, TEXT("Pressed from Char"));
	if (LocalFPSController != nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Local: Pressed from Char"));
		LocalFPSController->ToggleScoreboard();
	}
}

void AFPSCharacter::OnPressedSettings()
{
	UE_LOG(LogTemp, Warning, TEXT("Pressed from Char"));
	if (LocalFPSController != nullptr && IsLocallyControlled())
	{
		UE_LOG(LogTemp, Warning, TEXT("Local: Pressed from Char"));
		LocalFPSController->OpenSettingsMenu();
	}
}

void AFPSCharacter::OnAxisMoveForward(float Value)
{
	AddMovementInput(GetActorForwardVector(), Value);
}

void AFPSCharacter::OnAxisMoveRight(float Value)
{
	AddMovementInput(GetActorRightVector(), Value);
}

void AFPSCharacter::OnAxisLookUp(float Value)
{
	AddControllerPitchInput(Value);
}

void AFPSCharacter::OnAxisTurn(float Value)
{
	AddControllerYawInput(Value);
}

void AFPSCharacter::AddWeapon(TSubclassOf<AWeapon> DesiredWeaponClass)
{
	FActorSpawnParameters SpawnParameters = FActorSpawnParameters();

	SpawnParameters.Owner = this;
	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	Weapon = GetWorld()->SpawnActor<AWeapon>(DesiredWeaponClass, SpawnParameters);

	if (Weapon == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Weapon not created"));
		return;
	}

	//We are attaching it and since its Replicated it will attach to Mesh
	Weapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, "NormalGunPosition");
}

void AFPSCharacter::MulticastPlayAnimMontage_Implementation(UAnimMontage* AnimMontage)
{
	PlayAnimMontage(AnimMontage);
}

void AFPSCharacter::ClientPlaySound2D_Implementation(USoundBase* Sound)
{
	UGameplayStatics::PlaySound2D(GetWorld(), Sound);
}

//Remember the FireHitScan is a server call that is calling this function which removes the health which then gets replicated to the client
void AFPSCharacter::ApplyDamage(float Damage, AFPSCharacter* DamageCauser)
{
	if (IsDead())
	{
		return;
	}

	RemoveHealth(Damage);

	// Play the hit sound on the owning client of the DamageCauser

	if (HitSound != nullptr && DamageCauser != nullptr)
	{
		DamageCauser->ClientPlaySound2D(HitSound);
	}

	if (IsDead())
	{
		if (GameMode != nullptr)
		{
			//Since we are already on the server this GameMode will be valid
			GameMode->OnKill(DamageCauser->GetController(), GetController());
		}
	}
	else
	{
		ClientPlaySound2D(PainSound);
	}
}

void AFPSCharacter::FellOutOfWorld(const UDamageType& DmgType)
{
	// We don't want to call the parent version because it will immediately destroy the actor. We want to execute the OnKill logic in the game mode.

	if (GameMode != nullptr)
	{
		GameMode->OnKill(nullptr, GetController());
	}
}

void AFPSCharacter::Landed(const FHitResult& Hit)
{
	Super::Landed(Hit);
	if (LocalFPSController && IsLocallyControlled())
		LocalFPSController->OwningClientPlaySound(LandSound);
}