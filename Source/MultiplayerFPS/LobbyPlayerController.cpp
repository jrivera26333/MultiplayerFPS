// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyPlayerController.h"

void ALobbyPlayerController::PreClientTravel(const FString& PendingURL, ETravelType TravelType, bool bIsSeamlessTravel)
{
	Super::PreClientTravel(PendingURL, TravelType, bIsSeamlessTravel);

	FString SeamlessString = bIsSeamlessTravel ? "Is Seamless" : "Is not Seamlss";
	FString TravelTypeString;

	switch (TravelType)
	{
	case TRAVEL_Absolute:
		TravelTypeString = "Absolute";
		break;
	case TRAVEL_Partial:
		TravelTypeString = "Partial";
		break;
	case TRAVEL_Relative:
		TravelTypeString = "Relative";
		break;
	case TRAVEL_MAX:
		TravelTypeString = "Max";
		break;
	default:
		break;
	}

	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("Pending URL: %s Travel Type: %s bIsSeamlessTravel: %s"), *PendingURL, *TravelTypeString, *SeamlessString));
}