#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "FPSPlayerState.generated.h"

UCLASS()
class MULTIPLAYERFPS_API AFPSPlayerState : public APlayerState
{
	GENERATED_BODY()
	
protected:

	UPROPERTY(Replicated)
	int32 Kills = 0;

	UPROPERTY(Replicated)
	int32 Deaths = 0;

public:

	void AddKill() { ++Kills; }
	void AddDeath() { ++Deaths; }

	UFUNCTION(BlueprintCallable)
	int32 GetKills() const { return Kills; }
};