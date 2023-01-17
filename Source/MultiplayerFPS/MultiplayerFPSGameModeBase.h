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

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Multiplayer FPS Game Mode")
	int32 KillLimit = 30;

	AMultiplayerFPSGameModeBase();

	virtual bool ShouldSpawnAtStartSpot(AController* Player) override;
	virtual void HandleMatchHasStarted() override;
	virtual void HandleMatchHasEnded() override;
	virtual bool ReadyToEndMatch_Implementation() override;

	//Called after a successful login.This is the first place it is safe to call replicated functions on the PlayerController.
	virtual void PostLogin(APlayerController* NewPlayer) override;

	void RestartMap();
	virtual AActor* ChoosePlayerStart_Implementation(AController* Player) override;

	AActor* InitialSpawn(class AFPSPlayerController* LoggedInPlayer);

public:

	bool HasWinner() const;
	void OnKill(AController* KillerController, AController* VictimController);

private:
	void GetFarthestPlayerStart();
	void FindPlayerStarts();

	UPROPERTY()
	TArray<AActor*> PlayerStarts;

	UPROPERTY()
	TArray<APlayerController*> ConnectedPlayers;

	bool HasSpawnedCharacter;

	void StartGame();

	UPROPERTY()
	AActor* LastKnownKiller;

	UPROPERTY()
	class AFPSGameState* CurrentGameState;

	TArray<int> SpawnedIndexes;

	uint32 NumberOfPlayers = 0;
	uint32 SpawnCounter = 0;

	FTimerHandle GameStartTimer;
};