// Fill out your copyright notice in the Description page of Project Settings.


#include "HostInfoRow.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"

bool UHostInfoRow::Initialize()
{
	bool Success = Super::Initialize();
	if (!Success) return false;

	ServerButton->OnClicked.AddDynamic(this, &UHostInfoRow::ServerButtonClicked);

	return true;
}

void UHostInfoRow::SetRowTexts(const FString& ServerName, const FString& HostName, const FString& PlayersAmount)
{
	ServerNameText->SetText(FText::FromString(ServerName));
	HostNameText->SetText(FText::FromString(HostName));
	PlayersAmountText->SetText(FText::FromString(PlayersAmount));
}

void UHostInfoRow::ServerButtonClicked()
{
	UE_LOG(LogTemp, Warning, TEXT("Server Button Clicked!"));
}