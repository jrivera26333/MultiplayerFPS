#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "FPSGameState.generated.h"

UCLASS()
class MULTIPLAYERFPS_API AFPSGameState : public AGameState
{
	GENERATED_BODY()
	
public:

	//Referenced in Widget
	UFUNCTION(BlueprintCallable)
	TArray<class AFPSPlayerState*> GetPlayerStatesOrderedByKills() const;
};
