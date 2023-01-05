// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BaseMenu.h"
#include "JoinMenu.generated.h"

/**
 * 
 */
UCLASS()
class MULTIPLAYERFPS_API UJoinMenu : public UBaseMenu
{
	GENERATED_BODY()

protected:

	virtual bool Initialize() override;

	UPROPERTY(meta = (BindWidget))
	class UButton* JoinEnter;

	UPROPERTY(meta = (BindWidget))
	class UButton* JoinCancel;

private:
	
	UFUNCTION()
	void JoinEnterClicked();

	UFUNCTION()
	void JoinCancelClicked();

	UPROPERTY(meta = (BindWidget))
	class UScrollBox* RollScrollBox;

	TSubclassOf<UUserWidget> JoinRowWidgetClass;
	class UHostInfoRow* JoinRowWidget;

	APlayerController* PlayerController;

public:

	void SetRowWidget(TSubclassOf<UUserWidget> JoinRow) { JoinRowWidgetClass = JoinRow; };
	void SetServerList(TArray<FString> ServerNames);
};
