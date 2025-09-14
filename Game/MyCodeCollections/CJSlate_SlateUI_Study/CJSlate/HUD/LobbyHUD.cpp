// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyHUD.h"
#include "CJSlate/UI/LobbyPanelWidget.h"
#include "Blueprint/UserWidget.h"
#include "CJSlate/GameInstance/CJSlateGameInstance.h"

ALobbyHUD::ALobbyHUD()
{
	bReplicates = true;
}

void ALobbyHUD::BeginPlay()
{
	Super::BeginPlay();
	
	if (ULobbyPanelWidget* widget = CreateWidget<ULobbyPanelWidget>(GetWorld(), HUDPanelWidgetClass))
	{
		HUDPannelWidget = widget;
		HUDPannelWidget->AddToViewport();
	}
}