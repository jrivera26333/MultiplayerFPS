#include "FPSGameState.h"
#include "Net/UnrealNetwork.h"
#include "FPSPlayerState.h"

TArray<AFPSPlayerState*> AFPSGameState::GetPlayerStatesOrderedByKills() const
{
	TArray<AFPSPlayerState*> PlayerStates;

	for (APlayerState* PlayerState : PlayerArray)
	{
		AFPSPlayerState* FPSPlayerState = Cast<AFPSPlayerState>(PlayerState);

		PlayerStates.Add(FPSPlayerState);
	}

	PlayerStates.Sort([](const AFPSPlayerState& A, const AFPSPlayerState& B) { return A.GetKills() > B.GetKills(); });

	return PlayerStates;
}