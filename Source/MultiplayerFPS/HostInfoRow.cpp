// Fill out your copyright notice in the Description page of Project Settings.


#include "HostInfoRow.h"
#include "Components/Button.h"
#include "MainMenu.h"

void UHostInfoRow::Setup(class UMainMenu* InParent, uint32 InIndex)
{
	Parent = InParent;
	Index = InIndex;
	RowButton->OnClicked.AddDynamic(this, &UHostInfoRow::OnClicked);
}

void UHostInfoRow::OnClicked()
{
	Parent->SelectIndex(Index);
}