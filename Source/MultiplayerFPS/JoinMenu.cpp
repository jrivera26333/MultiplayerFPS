// Fill out your copyright notice in the Description page of Project Settings.


#include "JoinMenu.h"
#include "Components/Button.h"
#include "Components/WidgetSwitcher.h"
#include "Kismet/GameplayStatics.h"
#include "Components/ScrollBox.h"
#include "HostInfoRow.h"

bool UJoinMenu::Initialize()
{
	bool Success = Super::Initialize();
	if (!Success) return false;

	JoinEnter->OnClicked.AddDynamic(this, &UJoinMenu::JoinEnterClicked);
	JoinCancel->OnClicked.AddDynamic(this, &UJoinMenu::JoinCancelClicked);

	PlayerController = UGameplayStatics::GetPlayerController(this, 0);
	return true;
}

void UJoinMenu::JoinCancelClicked()
{
	UE_LOG(LogTemp, Warning, TEXT("Join Cancel Pressed"));

	if(GetWidgetSwitcher())
		GetWidgetSwitcher()->SetActiveWidgetIndex(0);
}

void UJoinMenu::JoinEnterClicked()
{
	UE_LOG(LogTemp, Warning, TEXT("Join Enter Pressed"));

	if (JoinRowWidgetClass)
	{
		JoinRowWidget = CreateWidget<UHostInfoRow>(PlayerController, JoinRowWidgetClass);
		RollScrollBox->AddChild(JoinRowWidget);
		UE_LOG(LogTemp, Warning, TEXT("Class not null!"));
	}
	else
		UE_LOG(LogTemp, Warning, TEXT("Class null!"));

	if (GetFPSGameInstance())
	{
		GetFPSGameInstance()->DebugSession();
	}
}