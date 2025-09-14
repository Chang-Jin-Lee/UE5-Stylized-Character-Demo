// Fill out your copyright notice in the Description page of Project Settings.


#include "MainUI.h"

#include "OptionMenu.h"
#include "RoundResultPanel.h"
#include "ShopPanel.h"
#include "ShopPanelSlot.h"
#include "Blueprint/WidgetTree.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"

TSharedRef<SWidget> UMainUI::RebuildWidget()
{
	UCanvasPanel* RootPanel = WidgetTree->ConstructWidget<UCanvasPanel>(UCanvasPanel::StaticClass(), TEXT("RootCanvas"));
	WidgetTree->RootWidget = RootPanel;

	SetDesiredSizeInViewport(FVector2D(1920.f, 1080.f));
	SetRenderTransformPivot(FVector2d(0.5f, 0.5f));

	RoundResultPanel = WidgetTree->ConstructWidget<URoundResultPanel>(URoundResultPanel::StaticClass(), TEXT("RoundResultPanel"));
	// RoundResultPanel = CreateWidget<URoundResultPanel>(this, URoundResultPanel::StaticClass());
	UCanvasPanelSlot* RoundResultPanelSlot = RootPanel->AddChildToCanvas(RoundResultPanel);
	RoundResultPanelSlot->SetMinimum(FVector2D(0.5f,0.0f));
	RoundResultPanelSlot->SetMaximum(FVector2D(0.5f,0.0f));
	RoundResultPanelSlot->SetAlignment(FVector2d(0.5f,0.f));
	RoundResultPanelSlot->SetSize(FVector2D(800, 80.f));
	RoundResultPanelSlot->SetPosition(FVector2d(0.f,0.0f));

	ShopPanel = WidgetTree->ConstructWidget<UShopPanel>(UShopPanel::StaticClass(), TEXT("ShopPanel"));
	UCanvasPanelSlot* ShopPanelSlot = RootPanel->AddChildToCanvas(ShopPanel);
	ShopPanelSlot->SetMinimum(FVector2D(0.5f,1.0f));
	ShopPanelSlot->SetMaximum(FVector2D(0.5f,1.0f));
	ShopPanelSlot->SetAlignment(FVector2d(0.5f,1.f));
	ShopPanelSlot->SetSize(FVector2D(1700, 230.0f));
	ShopPanelSlot->SetPosition(FVector2d(0.f,0.0f));
	
	return Super::RebuildWidget();
}
