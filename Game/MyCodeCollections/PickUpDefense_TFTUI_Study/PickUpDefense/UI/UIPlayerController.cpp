// Fill out your copyright notice in the Description page of Project Settings.


#include "UIPlayerController.h"

#include "MainUI.h"
#include "RoundResultPanel.h"

AUIPlayerController::AUIPlayerController()
{
	
}


void AUIPlayerController::BeginPlay()
{
	Super::BeginPlay();

	UMainUI* Widget = CreateWidget<UMainUI>(this, UMainUI::StaticClass());
	if(Widget)
	{
		Widget->AddToViewport();
	}

	FInputModeGameAndUI();
	bShowMouseCursor = true;
	bEnableClickEvents = true;
}
