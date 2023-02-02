#include "FPSPlayerState.h"
#include "Net/UnrealNetwork.h"
#include "FPSPlayerController.h"

void AFPSPlayerState::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AFPSPlayerState, Kills);
	DOREPLIFETIME(AFPSPlayerState, Deaths);
}