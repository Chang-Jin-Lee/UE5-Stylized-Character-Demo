// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/NameTagWidget.h"

#include "Blueprint/WidgetTree.h"
#include "Components/TextBlock.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"

TSharedRef<SWidget> UNameTagWidget::RebuildWidget()
{
	// Canvas Panel 생성
	UCanvasPanel* RootPanel = WidgetTree->ConstructWidget<UCanvasPanel>(UCanvasPanel::StaticClass(), TEXT("RootCanvas"));
	WidgetTree->RootWidget = RootPanel;

	// DPI 스케일을 1.0으로 설정
	SetDesiredSizeInViewport(FVector2D(250.f, 100.f));
	SetRenderTransformPivot(FVector2D(0.5f, 0.5f));

	// 텍스트 블록 생성
	NameTextBlock = WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass(), TEXT("NameTextBlock"));
	FSlateFontInfo FontInfo = FCoreStyle::GetDefaultFontStyle("Regular", 28);
	NameTextBlock->SetFont(FontInfo); // 폰트 적용
	NameTextBlock->SetColorAndOpacity(FLinearColor(0.95,0.06,1,1));

	// 텍스트 블록을 캔버스 패널에 추가하고 중앙에 배치
	UCanvasPanelSlot* TextSlot = RootPanel->AddChildToCanvas(NameTextBlock);
	TextSlot->SetAnchors(FAnchors(0.5f, 0.5f));
	TextSlot->SetAlignment(FVector2D(0.5f, 0.5f));
	TextSlot->SetAutoSize(true);  // 텍스트 블록이 자동으로 크기에 맞추어지도록 설정

	return Super::RebuildWidget();
}