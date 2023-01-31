#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "GameFramework/PlayerStart.h"
#include "MultiplayerFPSGameModeBase.generated.h"

UCLASS()
class MULTIPLAYERFPS_API AMultiplayerFPSGameModeBase : public AGameMode
{
	GENERATED_BODY()

protected:

	UPROPERTY(EditDefaultsOnly, Category = "Character")
	TSubclassOf<class AFPSMachineGunSoldier> MachineGunSoldierClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Multiplayer FPS Game Mode")
	int32 KillLimit = 30;

	AMultiplayerFPSGameModeBase();

	void RestartMap();

	virtual void HandleMatchHasEnded() override;

	virtual bool ReadyToEndMatch_Implementation() override;
	virtual bool ShouldSpawnAtStartSpot(AController* Player) override;

	//We are traveling from the Lobby so this will fire since we are switching gamemodes
	virtual void PostSeamlessTravel() override;


public:

	void OnKill(AController* KillerController, AController* VictimController);
	bool HasWinner() const;
	void AddToCurrentPlayersLoading(class AFPSPlayerController* PlayerController);

	//Test (place in private)
	void SpawnPlayerTest(class AFPSPlayerController* PlayerController);

private:

	bool HasStartedTraveling;
	bool HasSpawnedPlayers;

	void StartGame();
	void FindPlayerStarts();

	UPROPERTY()
	TArray<AActor*> PlayerStarts;

	UPROPERTY()
	TArray<class AFPSPlayerController*> PlayersLoggedIn;

	UPROPERTY()
	class AFPSGameState* CurrentGameState;

	uint32 CurrentPlayersLoaded = 0;
	FTimerHandle GameStartTimer;
};