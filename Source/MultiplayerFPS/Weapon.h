#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnumTypes.h"
#include "Weapon.generated.h"

UCLASS()
class MULTIPLAYERFPS_API AWeapon : public AActor
{
	GENERATED_BODY()

public:
	void Reload();
	virtual void SetOwner(AActor* NewOwner) override;

protected:

	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon")
	USkeletalMeshComponent* Mesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	FName Name;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	EWeaponType WeaponType;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	EAmmoType AmmoType;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	EWeaponFireMode FireMode;

	//Replicating because we are firing our gun on the server which then needs to update the bullets on the client. Remember the call is only happening on the Server
	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Weapon")
	float CurrentAmmo = 60.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	float AmmoClipSize = 60.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	float HitscanRange = 9999.9f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	float HitscanDamage = 20.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	float FireRate = 1.0f;

	UPROPERTY(Replicated)
	bool IsReloading;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	UAnimMontage* FireAnimMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	UAnimMontage* ReloadAnimMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	USoundBase* NoAmmoSound;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	class UParticleSystem* MuzzleFlash;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	class UParticleSystem* HitFlash;

	FTimerHandle FireTimer;

	bool bWantsFire;

	class AFPSCharacter* Character;

	AWeapon();

	void StartFire();

	void FireHitscan(FVector FireLocation, FVector FireDirection);

	UFUNCTION(Server, Reliable)
	void ServerStartFire();

	UFUNCTION(Server, Reliable)
	void ServerStopFire();

	UFUNCTION(Server, Reliable, BlueprintCallable)
	void ServerReload();

	//UFUNCTION(BlueprintCallable)
	//void ReloadWeapon();

	UFUNCTION(BlueprintCallable, BlueprintPure)
	float GetCurrentAmmo() const { return CurrentAmmo; };

	UFUNCTION(NetMulticast, Unreliable)
	void MulticastMuzzleEmitter();

	UFUNCTION(NetMulticast, Unreliable)
	void MulticastHitEmitter(FVector PointOfImpact);

	bool HasEnoughAmmo();

public:
	virtual void OnPressedFire();
	virtual void OnReleasedFire();
};
