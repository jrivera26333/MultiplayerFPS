#include "Weapon.h"
#include "FPSCharacter.h"
#include "Net/UnrealNetwork.h"
#include "Particles/ParticleSystem.h"
#include "Kismet/GameplayStatics.h"

//Replicated Weapon
AWeapon::AWeapon()
{
	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>("Mesh");
	RootComponent = Mesh;

	bReplicates = true;
}

void AWeapon::BeginPlay()
{
	Super::BeginPlay();
}

void AWeapon::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(AWeapon, CurrentAmmo, COND_OwnerOnly);
	DOREPLIFETIME_CONDITION(AWeapon, IsReloading, COND_OwnerOnly);
}

void AWeapon::SetOwner(AActor* NewOwner)
{
	Super::SetOwner(NewOwner);
	Character = Cast<AFPSCharacter>(NewOwner);
}

/// <summary>
/// Being called from the Server RPC
/// </summary>
void AWeapon::StartFire()
{
	// Abort if the trigger is no longer down OR if there is still time remaining in the FireTimer
	if (!bWantsFire || GetWorldTimerManager().GetTimerRemaining(FireTimer) > 0.0f || IsReloading)
		return;

	// If there is no ammo, play the no ammo sound on the owning client of the owning character and abort

	if (!HasEnoughAmmo())
		return;

	CurrentAmmo--;

	const FVector FireLocation = Character->GetCameraLocation();
	const FVector FireDirection = Character->GetCameraDirection();

	FireHitscan(FireLocation, FireDirection);

	// Play the fire anim montage in all of the instances of the owning character
	if (FireAnimMontage != nullptr)
		Character->MulticastPlayAnimMontage(FireAnimMontage);


	if (MuzzleFlash != nullptr)
		MulticastMuzzleEmitter();

	//Schedule the FireTimer depending on the value of the FireMode

	if (FireMode == EWeaponFireMode::Automatic && bWantsFire)
	{
		GetWorldTimerManager().SetTimer(FireTimer, this, &AWeapon::StartFire, FireRate);
	}
	else if (FireMode == EWeaponFireMode::Single)
	{
		GetWorldTimerManager().SetTimer(FireTimer, FireRate, false);
	}

	//Reusing in the case people are holding down the trigger
	if (!HasEnoughAmmo())
		return;
}

bool AWeapon::HasEnoughAmmo()
{
	if (CurrentAmmo <= 0)
	{
		if (NoAmmoSound != nullptr)
		{
			Character->ClientPlaySound2D(NoAmmoSound);
			ServerReloadFire();
		}

		return false;
	}

	return true;
}

void AWeapon::FireHitscan(FVector FireLocation, FVector FireDirection)
{
	// Perform a line trace from the FireLocation and towards the FireDirection multiplied by the HitscanRange, that ignores the owning character

	FHitResult Hit(ForceInit);
	FCollisionQueryParams TraceParams("Fire Trace", false, Character);

	const FVector Start = FireLocation;
	const FVector End = Start + FireDirection * HitscanRange;

	GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility, TraceParams);

	// Apply damage if the hit actor is a AFPSCharacter

	AFPSCharacter* HitCharacter = Cast<AFPSCharacter>(Hit.Actor.Get());

	MulticastHitEmitter(Hit.ImpactPoint);

	if (HitCharacter != nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Hit Character!"));
		HitCharacter->ApplyDamage(HitscanDamage, Character);
	}
}

void AWeapon::MulticastMuzzleEmitter_Implementation()
{
	if (MuzzleFlash != nullptr)
	{
		UGameplayStatics::SpawnEmitterAttached(MuzzleFlash, Mesh, TEXT("Muzzle"));
	}
}

void AWeapon::MulticastHitEmitter_Implementation(FVector PointOfImpact)
{
	if (HitFlash != nullptr)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitFlash, PointOfImpact);
	}
}

//Server RPC's

void AWeapon::ServerReload_Implementation()
{
	CurrentAmmo = AmmoClipSize;
	IsReloading = false;

	UE_LOG(LogTemp, Warning, TEXT("Reloaded From"));
}

void AWeapon::ServerStartFire_Implementation()
{
	UE_LOG(LogTemp, Warning, TEXT("Server Fired!"));
	bWantsFire = true;
	StartFire();
}

void AWeapon::ServerStopFire_Implementation()
{
	bWantsFire = false;
}

void AWeapon::ServerReloadFire_Implementation()
{
	// Weapon is being replicated from the Server to the Client. The bool is being toggled in the ReloadNotify

	if (ReloadAnimMontage != nullptr && !IsReloading)
	{
		//Playing Reload Montage
		IsReloading = true;
		Character->MulticastPlayAnimMontage(ReloadAnimMontage);
	}
}

//Attached Server RPC's
void AWeapon::OnPressedFire()
{
	ServerStartFire();
}

void AWeapon::OnReleasedFire()
{
	ServerStopFire();
}

void AWeapon::OnPressedReload()
{
	ServerReloadFire();
}