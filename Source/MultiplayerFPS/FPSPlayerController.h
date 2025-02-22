#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "FPSPlayerController.generated.h"

USTRUCT(BlueprintType)
struct FAbilitySets
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "FPS Player Controller")
	UTexture2D* AbilityTexture;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "FPS Player Controller")
	FString AbilityInput;
};

UCLASS()
class MULTIPLAYERFPS_API AFPSPlayerController : public APlayerController
{
	GENERATED_BODY()

protected:

	UPROPERTY()
	class UPlayerMenu* PlayerMenu;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "FPS Character")
	USoundBase* TestSound;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "FPS Player Controller")
	TSubclassOf<class UPlayerMenu> PlayerMenuClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "FPS Player Controller")
	TArray<FAbilitySets> Abilities;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "FPS Player Controller")
	UTexture2D* WeaponPortrait;
	
	virtual void PostSeamlessTravel() override;

public:

	void ToggleScoreboard();
	void OpenSettingsMenu();

	void AddAbilityPortraits();
	void AddWeaponPortrait();

	UFUNCTION(BlueprintCallable)
	void OwningClientPlaySound(USoundBase* Sound);

	//RPC's
	UFUNCTION(Client, Reliable)
	void ClientNotifyKill(const FString& Name);

	UFUNCTION(Client, Reliable)
	void ClientUpdateKillsTarget(const FString& Name);

	UFUNCTION(Client, Reliable)
	void ClientShowScoreboard();

	UFUNCTION(Client, Reliable)
	void ClientUpdatePlayersUI();

	UFUNCTION(Client, Reliable)
	void ClientCreatePlayerMenuWidget();

	UFUNCTION(Client, Reliable)
	void ClientRefreshKills();
};