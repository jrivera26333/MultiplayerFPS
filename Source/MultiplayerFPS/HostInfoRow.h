// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HostInfoRow.generated.h"

/**
 * 
 */
UCLASS()
class MULTIPLAYERFPS_API UHostInfoRow : public UUserWidget
{
	GENERATED_BODY()

public:

	void SetRowTexts(const FString& ServerName, const FString& HostName, const FString& PlayersAmount);
	
protected:

	virtual bool Initialize() override;

	UPROPERTY(meta = (BindWidget))
	class UButton* ServerButton;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* ServerNameText;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* HostNameText;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* PlayersAmountText;

private:

	UFUNCTION()
	void ServerButtonClicked();
};
