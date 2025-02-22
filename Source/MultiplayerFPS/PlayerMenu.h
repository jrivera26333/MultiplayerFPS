#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerMenu.generated.h"

UCLASS()
class MULTIPLAYERFPS_API UPlayerMenu : public UUserWidget
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintImplementableEvent)
	void ToggleScoreboard();

	UFUNCTION(BlueprintImplementableEvent)
	void OpenSettingsMenu();

	UFUNCTION(BlueprintImplementableEvent)
	void SetupPlayersUI();

	UFUNCTION(BlueprintImplementableEvent)
	void RefreshKillsUI();

	UFUNCTION(BlueprintImplementableEvent)
	void SetScoreboardVisibility(bool bIsVisible);

	UFUNCTION(BlueprintImplementableEvent)
	void NotifyKill(const FString& Name);

	UFUNCTION(BlueprintImplementableEvent)
	void UpdateKillsTarget(const FString& Name);

	UFUNCTION(BlueprintImplementableEvent)
	void SetAbilityIcons(FString const& AbilityInput, UTexture2D* const& AbilityTexture);

	UFUNCTION(BlueprintImplementableEvent)
	void SetWeaponPortrait(UTexture2D* const& AbilityTexture);

	UFUNCTION(BlueprintImplementableEvent)
	void SetCharacterReference();
};
