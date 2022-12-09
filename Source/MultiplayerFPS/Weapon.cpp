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

	PrimaryActorTick.bCanEverTick = false;
}

void AWeapon::BeginPlay()
{
	Super::BeginPlay();
	ReloadWeapon();
}

void AWeapon::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(AWeapon, CurrentAmmo, COND_OwnerOnly);
}

void AWeapon::SetOwner(AActor* NewOwner)
{
	Super::SetOwner(NewOwner);
	Character = Cast<AFPSCharacter>(NewOwner);
	
	if (Character != nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Not null"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Null"));
	}
}

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
	{
		Character->MulticastPlayAnimMontage(FireAnimMontage);
	}

	if (MuzzleFlash != nullptr)
	{
		MulticastMuzzleEmitter();
	}

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
			//Sending an RPC to the Client (Remember value is not being replicated its an audio clip)
			Character->ClientPlaySound2D(NoAmmoSound);

			if (ReloadAnimMontage != nullptr)
			{
				//Playing Reload Montage
				Character->MulticastPlayAnimMontage(ReloadAnimMontage);
				IsReloading = true;
			}
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

//Is being called by a notify so we need to update the server value for the ammunition
void AWeapon::ReloadWeapon()
{
	CurrentAmmo = AmmoClipSize;
	ServerReload();

	//We had the IsReloading here however we are firing the weapon on the server so the bool was only getting changed on the client, so we added it to the ServerReload RPC
}

void AWeapon::ServerReload_Implementation()
{
	CurrentAmmo = AmmoClipSize;
	IsReloading = false;
}

void AWeapon::ServerStartFire_Implementation()
{
	bWantsFire = true;
	StartFire();
}

void AWeapon::ServerStopFire_Implementation()
{
	bWantsFire = false;
}

//Attached Server RPC's
void AWeapon::OnPressedFire()
{
	//Calling Implementation
	ServerStartFire();
}

//Attached Server RPC's
void AWeapon::OnReleasedFire()
{
	//Calling Implementation
	ServerStopFire();
}