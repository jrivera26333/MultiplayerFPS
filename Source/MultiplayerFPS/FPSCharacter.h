#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MultiplayerFPS.h"
#include "EnumTypes.h"
#include "Weapon.h"
#include "CharacterSetupInterface.h"
#include "Camera/CameraComponent.h"
#include "FPSCharacter.generated.h"

UCLASS()
class MULTIPLAYERFPS_API AFPSCharacter : public ACharacter, public ICharacterSetupInterface
{
	GENERATED_BODY()

protected:
	// Components

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "FPS Character")
	class UCameraComponent* CameraComponent;

	// Health

	UPROPERTY(Replicated, BlueprintReadOnly, Category = "FPS Character")
	float Health = 0.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "FPS Character")
	float MaxHealth = 100.0f;

	// Character

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "FPS Character")
	FString CharacterName;

	// Armor

	UPROPERTY(Replicated, BlueprintReadOnly, Category = "FPS Character")
	float Armor = 0.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "FPS Character")
	float MaxArmor = 100.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "FPS Character")
	float ArmorAbsorption = 0.5;

	// Weapons

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "FPS Character")
	TSubclassOf<AWeapon> WeaponClass;

	UPROPERTY(Replicated)
	AWeapon* Weapon;

	int32 WeaponIndex = INDEX_NONE;

	// Sounds

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "FPS Character")
	USoundBase* SpawnSound;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "FPS Character")
	USoundBase* HitSound;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "FPS Character")
	USoundBase* WeaponChangeSound;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "FPS Character")
	USoundBase* PainSound;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "FPS Character")
	USoundBase* LandSound;

	// Game Mode

	UPROPERTY()
	class AMultiplayerFPSGameModeBase* GameMode;

	// Constructor and overrided

	AFPSCharacter();

	virtual void BeginPlay() override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void FellOutOfWorld(const UDamageType& DmgType) override;

	virtual void Landed(const FHitResult& Hit) override;

	// Input

	void OnPressedJump();

	void OnPressedFire();
	void OnReleasedFire();

	void OnPressedScoreboard();

	void OnAxisMoveForward(float Value);
	void OnAxisMoveRight(float Value);
	void OnAxisLookUp(float Value);
	void OnAxisTurn(float Value);

	// Getters
	
	UFUNCTION(BlueprintCallable, BlueprintPure)
	AWeapon* GetWeapon() const { return Weapon; };
	
	// Interface

	UFUNCTION()
	virtual void SecondaryAbility(){UE_LOG(LogTemp, Warning, TEXT("Ability not set")) };

public:

	// Damage

	void ApplyDamage(float Damage, AFPSCharacter* DamageCauser);

	// RPCs

	UFUNCTION(NetMulticast, Unreliable)
	void MulticastPlayAnimMontage(UAnimMontage* AnimMontage);

	UFUNCTION(Client, Unreliable)
	void ClientPlaySound2D(USoundBase* Sound);

	// Health

	void AddHealth(float Amount) { SetHealth(Health + Amount); }
	void RemoveHealth(float Amount) { SetHealth(Health - Amount); }
	void SetHealth(float NewHealth) { Health = FMath::Clamp(NewHealth, 0.0f, MaxHealth); }
	bool IsDead() const { return Health == 0.0f; }

	// Armor

	void AddArmor(float Amount) { SetArmor(Armor + Amount); }
	void SetArmor(float Amount) { Armor = FMath::Clamp(Amount, 0.0f, MaxArmor); }
	void ArmorAbsorbDamage(float& Damage);

	// Weapons

	void AddWeapon(TSubclassOf<AWeapon> WeaponType);

	// Camera

	FVector GetCameraLocation() const { return CameraComponent->GetComponentLocation(); }
	FVector GetCameraDirection() const { return GetControlRotation().Vector(); }
};