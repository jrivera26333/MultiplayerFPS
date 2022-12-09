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

	if (!HasAuthority())
	{
		return;		
	}

	SetHealth(MaxHealth);

	AddWeapon(WeaponClass);
	UE_LOG(LogTemp, Warning, TEXT("Weapon: %s"), *WeaponClass);

	GameMode = Cast<AMultiplayerFPSGameModeBase>(GetWorld()->GetAuthGameMode());
}

void AFPSCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Bind the action mappings

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AFPSCharacter::OnPressedJump);

	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &AFPSCharacter::OnPressedFire);
	PlayerInputComponent->BindAction("Fire", IE_Released, this, &AFPSCharacter::OnReleasedFire);

	PlayerInputComponent->BindAction("Secondary Ability", IE_Pressed, this, &AFPSCharacter::SecondaryAbility);

	PlayerInputComponent->BindAction("Scoreboard", IE_Pressed, this, &AFPSCharacter::OnPressedScoreboard);

	// Bind the axis mappings

	PlayerInputComponent->BindAxis("MoveForward", this, &AFPSCharacter::OnAxisMoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AFPSCharacter::OnAxisMoveRight);
	PlayerInputComponent->BindAxis("LookUp", this, &AFPSCharacter::OnAxisLookUp);
	PlayerInputComponent->BindAxis("Turn", this, &AFPSCharacter::OnAxisTurn);
}

void AFPSCharacter::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(AFPSCharacter, Health, COND_OwnerOnly);
	DOREPLIFETIME_CONDITION(AFPSCharacter, Armor, COND_OwnerOnly);
	DOREPLIFETIME_CONDITION(AFPSCharacter, Weapon, COND_OwnerOnly);
}

void AFPSCharacter::OnPressedJump()
{
	Jump();
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

void AFPSCharacter::OnPressedScoreboard()
{
	AFPSPlayerController* PlayerController = Cast<AFPSPlayerController>(GetController());

	if (PlayerController != nullptr)
	{
		PlayerController->ToggleScoreboard();
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

void AFPSCharacter::ArmorAbsorbDamage(float& Damage)
{
	// Calculate how much damage was absorbed and subtract that from the Armor

	const float AbsorbedDamage = Damage * ArmorAbsorption;
	const float RemainingArmor = Armor - AbsorbedDamage;

	SetArmor(RemainingArmor);

	// Recalculate the damage

	Damage = (Damage * (1 - ArmorAbsorption)) - FMath::Min(RemainingArmor, 0.0f);
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
	Weapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, "GripPoint");
}

void AFPSCharacter::MulticastPlayAnimMontage_Implementation(UAnimMontage* AnimMontage)
{
	PlayAnimMontage(AnimMontage);
}

void AFPSCharacter::ClientPlaySound2D_Implementation(USoundBase* Sound)
{
	UGameplayStatics::PlaySound2D(GetWorld(), Sound);
}

void AFPSCharacter::ApplyDamage(float Damage, AFPSCharacter* DamageCauser)
{
	if (IsDead())
		return;

	// Deduct the armor and the health

	ArmorAbsorbDamage(Damage);

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

	UGameplayStatics::PlaySound2D(GetWorld(), LandSound);
}