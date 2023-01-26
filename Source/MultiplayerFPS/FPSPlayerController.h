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

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "FPS Character")
	USoundBase* TestSound;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "FPS Player Controller")
	TSubclassOf<class UPlayerMenu> PlayerMenuClass;

	UPROPERTY()
	class UPlayerMenu* PlayerMenu;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "FPS Player Controller")
	TArray<FAbilitySets> Abilities;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "FPS Player Controller")
	UTexture2D* WeaponPortrait;

	virtual void BeginPlay() override;

public:

	void ToggleScoreboard();
	void OpenSettingsMenu();

	//PlayerController and Widgets should communicate to each other so client communication is prefered
	UFUNCTION(Client, Reliable)
	void ClientNotifyKill(const FString& Name);	

	UFUNCTION(Client, Reliable)
	void ClientShowScoreboard();

	void AddAbilityPortraits();
	void AddWeaponPortrait();
	void ResetPlayerReference();
	void AddPlayerLoggedIn(class AFPSPlayerController* PlayerLogged) { PlayerLoggedIn.Add(PlayerLogged); };

	void SetPlayerNumber(int32 PlayerCount) { PlayerNumber = PlayerCount; };
	int32 GetPlayerNumber() { return PlayerNumber; };

	void SetHasSpawnBeenSet(bool SpawnSet) { HasSpawnBeenSet = SpawnSet; };
	bool GetHasSpawnBeenSet() { return HasSpawnBeenSet; };

	UFUNCTION(BlueprintCallable)
	void SetPlayerID(const FString& PlayerID) { SteamPlayerID = PlayerID; }

	UFUNCTION(BlueprintCallable)
	void OwningClientPlaySound(USoundBase* Sound);

private:
	
	int32 PlayerNumber;
	bool HasSpawnBeenSet;

	FString SteamPlayerID;

	UPROPERTY(ReplicatedUsing = UpdatePlayersUI)
	TArray<class AFPSPlayerController*> PlayerLoggedIn;

	UFUNCTION()
	void UpdatePlayersUI();
};