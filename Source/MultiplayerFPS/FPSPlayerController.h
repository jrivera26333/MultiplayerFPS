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
	virtual void PostSeamlessTravel() override;

public:

	void ToggleScoreboard();
	void OpenSettingsMenu();

	//PlayerController and Widgets should communicate to each other so client communication is prefered
	UFUNCTION(Client, Reliable)
	void ClientNotifyKill(const FString& Name);	

	UFUNCTION(Client, Reliable)
	void ClientShowScoreboard();

	UFUNCTION(Client, Reliable)
	void ClientUpdatePlayersUI();

	UFUNCTION(Client, Reliable)
	void CreatePlayerMenuWidget();

	UFUNCTION(Server, Reliable)
	void ServerGetPlayerNames(const TArray<FString>& PlayerNamesRecieved);

	UFUNCTION(Server, Reliable)
	void ServerUpdatedGMPlayerHasLoaded();

	UFUNCTION()
	void UpdatePlayersUI();

	void AddAbilityPortraits();
	void AddWeaponPortrait();
	void ResetPlayerReference();

	void SetPlayerNumber(int32 PlayerCount) { PlayerNumber = PlayerCount; };
	int32 GetPlayerNumber() { return PlayerNumber; };

	void SetHasSpawnBeenSet(bool SpawnSet) { HasSpawnBeenSet = SpawnSet; };
	bool GetHasSpawnBeenSet() { return HasSpawnBeenSet; };

	void SetPlayerID(const FString& PlayerID) { SteamPlayerID = PlayerID; }

	UFUNCTION(BlueprintCallable)
	void OwningClientPlaySound(USoundBase* Sound);

	UFUNCTION(BlueprintCallable)
	TArray<FString> GetPlayerNamesLoggedIn() const { return PlayerNamesLoggedIn; };
	void AddPlayersLoggedIn(FString PlayerLogged) { PlayerNamesLoggedIn.Add(PlayerLogged); };

private:
	
	int32 PlayerNumber;
	bool HasSpawnBeenSet;

	FString SteamPlayerID;

	UPROPERTY(ReplicatedUsing = UpdatePlayersUI)
	TArray<FString> PlayerNamesLoggedIn;
};