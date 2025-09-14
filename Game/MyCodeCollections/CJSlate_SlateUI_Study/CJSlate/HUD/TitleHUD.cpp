// Fill out your copyright notice in the Description page of Project Settings.


#include "TitleHUD.h"

#include "Blueprint/UserWidget.h"
#include "CJSlate/UI/TitleScreenUI.h"

ATitleHUD::ATitleHUD()
{
}

void ATitleHUD::BeginPlay()
{
	Super::BeginPlay();
	
	if (UTitleScreenUI* widget = CreateWidget<UTitleScreenUI>(GetWorld(), HUDPanelWidgetClass))
	{
		HUDPanelWidget = widget;
		HUDPanelWidget->AddToViewport();
	}
}
