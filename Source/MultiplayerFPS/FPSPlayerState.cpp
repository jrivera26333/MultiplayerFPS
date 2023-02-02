#include "FPSPlayerState.h"
#include "Net/UnrealNetwork.h"
#include "FPSPlayerController.h"

void AFPSPlayerState::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AFPSPlayerState, Kills);
	DOREPLIFETIME(AFPSPlayerState, Deaths);
}

void AFPSPlayerState::OnRep_UpdateKillUI()
{
	AFPSPlayerController* Controller = Cast<AFPSPlayerController>(GetNetOwningPlayer());

	if (Controller == nullptr) return;

	if (Controller->IsLocalController())
	{
		Controller->ClientRefreshKills();
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("Refreshed Kills")));
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("Did not refresh kills")));
	}
}