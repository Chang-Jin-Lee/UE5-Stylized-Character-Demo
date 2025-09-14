// Fill out your copyright notice in the Description page of Project Settings.


#include "RoundResultPanel.h"
#include "Blueprint/WidgetTree.h"
#include "Components/Border.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/HorizontalBox.h"
#include "Components/HorizontalBoxSlot.h"
#include "Components/ProgressBar.h"
#include "Components/SizeBox.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"
#include "Components/VerticalBoxSlot.h"

TSharedRef<SWidget> URoundResultPanel::RebuildWidget()
{
	SetDesiredSizeInViewport(FVector2D(1000,100));
	SetRenderTransformPivot(FVector2D(0,0));

	USizeBox* SizeBox = WidgetTree->ConstructWidget<USizeBox>(USizeBox::StaticClass(), TEXT("SizeBox"));
	SizeBox->SetWidthOverride(1000);
	SizeBox->SetHeightOverride(100);
	WidgetTree->RootWidget = SizeBox;
	
	UVerticalBox* VerticalBox = WidgetTree->ConstructWidget<UVerticalBox>(UVerticalBox::StaticClass(), TEXT("VerticalBox"));
	SizeBox->AddChild(VerticalBox);
	UHorizontalBox* HorizontalBox = WidgetTree->ConstructWidget<UHorizontalBox>(UHorizontalBox::StaticClass(), TEXT("HorizontalBox"));
	UVerticalBoxSlot* HorizontalBoxSlot = VerticalBox->AddChildToVerticalBox(HorizontalBox);
	
	FSlateChildSize ChildSize;
	ChildSize.Value = 1.0f;
	HorizontalBoxSlot->SetSize(ChildSize);
	UProgressBar* ProgressBar = WidgetTree->ConstructWidget<UProgressBar>(UProgressBar::StaticClass(), TEXT("UProgressBar"));
	ProgressBar->SetFillColorAndOpacity(FLinearColor::FromSRGBColor(FColor::FromHex("#56c8c7")));
	ProgressBar->SetPercent(0.6);
	ProgressBar->BarFillType = EProgressBarFillType::RightToLeft;
	UVerticalBoxSlot* ProgressBarSlot = VerticalBox->AddChildToVerticalBox(ProgressBar);
	ProgressBarSlot->SetPadding(FMargin(50,10,50,0));

	UBorder* LeftBox = WidgetTree->ConstructWidget<UBorder>(UBorder::StaticClass(), TEXT("LeftBox"));
	LeftBox->SetPadding(FMargin(5,5,5,5));
	LeftBox->SetHorizontalAlignment(HAlign_Center);
	LeftBox->SetVerticalAlignment(VAlign_Center);
	LeftBox->SetBrushColor(FLinearColor::FromSRGBColor(FColor::FromHex(TEXT("#1a2d2a"))));
	
	FSlateBrush Brush;
	Brush.DrawAs = ESlateBrushDrawType::RoundedBox;
	FSlateBrushOutlineSettings OutlineSettings;
	OutlineSettings.Width = 0;
	OutlineSettings.RoundingType = ESlateBrushRoundingType::Type::FixedRadius;
	OutlineSettings.CornerRadii = FVector4(0,0,0,40);
	Brush.OutlineSettings = OutlineSettings;
	
	LeftBox->SetBrush(Brush);
	UHorizontalBoxSlot* LeftBoxSlot = HorizontalBox->AddChildToHorizontalBox(LeftBox);
	ChildSize.Value= 0.5f;
	LeftBoxSlot->SetSize(ChildSize);
	LeftBoxSlot->SetPadding(FMargin(5.0f, 0, 5.0f, 0));

	UBorder* MiddleBox = WidgetTree->ConstructWidget<UBorder>(UBorder::StaticClass(), TEXT("MiddleBox"));
	MiddleBox->SetPadding(FMargin(5,5,5,5));
	MiddleBox->SetHorizontalAlignment(HAlign_Center);
	MiddleBox->SetVerticalAlignment(VAlign_Center);
	MiddleBox->SetBrushColor(FLinearColor::FromSRGBColor(FColor::FromHex(TEXT("#171d1e"))));
	UHorizontalBoxSlot* MiddleBoxSlot = HorizontalBox->AddChildToHorizontalBox(MiddleBox);
	FSlateChildSize ChildSize2;
	ChildSize2.Value= 1.0f;
	MiddleBoxSlot->SetSize(ChildSize2);
	MiddleBoxSlot->SetPadding(FMargin(5.0f, 0, 5.0f, 0));

	UBorder* RightBox = WidgetTree->ConstructWidget<UBorder>(UBorder::StaticClass(), TEXT("RightBox"));
	RightBox->SetPadding(FMargin(5,5,5,5));
	RightBox->SetHorizontalAlignment(HAlign_Center);
	RightBox->SetVerticalAlignment(VAlign_Center);
	RightBox->SetBrushColor(FLinearColor::FromSRGBColor(FColor::FromHex(TEXT("#1a2d2a"))));
	
	OutlineSettings.CornerRadii = FVector4(0,0,40,0);
	Brush.OutlineSettings = OutlineSettings;
	
	RightBox->SetBrush(Brush);
	UHorizontalBoxSlot* RightBoxSlot = HorizontalBox->AddChildToHorizontalBox(RightBox);
	FSlateChildSize ChildSize3;
	ChildSize3.Value= 0.5f;
	RightBoxSlot->SetSize(ChildSize);
	RightBoxSlot->SetPadding(FMargin(5.0f, 0, 5.0f, 0));

	UTextBlock* TestText = WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass(), TEXT("Test"));
	TestText->SetText(FText::FromString("2-5"));
	FSlateFontInfo FontInfo = FCoreStyle::GetDefaultFontStyle("Regular", 26);
	TestText->SetFont(FontInfo);
	LeftBox->AddChild(TestText);

	UTextBlock* TestText2 = WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass(), TEXT("Test1"));
	TestText2->SetText(FText::FromString("This is Round Pannel"));
	FSlateFontInfo FontInfo2 = FCoreStyle::GetDefaultFontStyle("Regular", 26);
	TestText2->SetFont(FontInfo2);
	MiddleBox->AddChild(TestText2);
	
	UTextBlock* TestText3 = WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass(), TEXT("Test2"));
	TestText3->SetText(FText::FromString("Time 24"));
	FSlateFontInfo FontInfo3 = FCoreStyle::GetDefaultFontStyle("Regular", 26);
	TestText3->SetFont(FontInfo3);
	RightBox->AddChild(TestText3);

	// HorizontalBox->AddChild(LeftBox);
	// HorizontalBox->AddChild(MiddleBox);
	// HorizontalBox->AddChild(RightBox);
	
	return Super::RebuildWidget();
}
