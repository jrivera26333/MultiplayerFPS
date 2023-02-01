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

	virtual void Tick(float dt) override;

	UPROPERTY(EditDefaultsOnly, Category = "Character")
	TSubclassOf<class AFPSMachineGunSoldier> MachineGunSoldierClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Multiplayer FPS Game Mode")
	int32 KillLimit = 30;

	AMultiplayerFPSGameModeBase();

	void RestartMap();

	virtual void HandleMatchHasEnded() override;
	virtual void HandleMatchHasStarted() override;

	virtual bool ReadyToEndMatch_Implementation() override;
	virtual bool ShouldSpawnAtStartSpot(AController* Player) override;

	//We are traveling from the Lobby so this will fire since we are switching gamemodes
	virtual void PostSeamlessTravel() override;

	UPROPERTY(EditDefaultsOnly, Category = "Character Spawns")
	APlayerStart* P1PlayerStart;

	UPROPERTY(EditDefaultsOnly, Category = "Character Spawns")
	APlayerStart* P2PlayerStart;

public:

	void OnKill(AController* KillerController, AController* VictimController);
	void AddToCurrentPlayersLoading(class AFPSPlayerController* PlayerController);

private:

	bool HasStartedTraveling;
	bool HasSpawnedPlayers;

	UPROPERTY()
	TArray<AActor*> PlayerStarts;

	UPROPERTY()
	TArray<class AFPSPlayerStart*> CastedPlayerStarts;

	UPROPERTY()
	TArray<class AFPSPlayerController*> PlayersLoggedIn;

	UPROPERTY()
	class AFPSGameState* CurrentGameState;

	uint32 CurrentPlayersLoaded = 0;
	FTimerHandle GameStartTimer;

	bool HasWinner() const;
	void FindPlayerStarts();
	void InitialSpawnPlayer(class AFPSPlayerController* PlayerController);
};