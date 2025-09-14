// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/InventoryHoverWidget.h"

#include "Blueprint/WidgetTree.h"
#include "Components/Border.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/HorizontalBox.h"
#include "Components/HorizontalBoxSlot.h"
#include "Components/Image.h"
#include "Components/SizeBox.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"

TSharedRef<SWidget> UInventoryHoverWidget::RebuildWidget()
{
	SetDesiredSizeInViewport(FVector2D(400,200));
	SetRenderTransformPivot(FVector2D(0, 0));
	
	USizeBox* SizeBox = WidgetTree->ConstructWidget<USizeBox>(USizeBox::StaticClass());
	SizeBox->SetWidthOverride(400);
	SizeBox->SetHeightOverride(200);
	WidgetTree->RootWidget = SizeBox;

	UBorder* Border = WidgetTree->ConstructWidget<UBorder>(UBorder::StaticClass());
	Border->SetBrushColor(FLinearColor(0.f, 0.f, 0.f, 0.0f));
	SizeBox->AddChild(Border);
	
	UHorizontalBox* HorizontalBox = WidgetTree->ConstructWidget<UHorizontalBox>(UHorizontalBox::StaticClass());
	Border->AddChild(HorizontalBox);

	UImage* Image = WidgetTree->ConstructWidget<UImage>(UImage::StaticClass());
	Image->SetBrushFromTexture(ItemData.ItemImage);
	UHorizontalBoxSlot* ImageSlot = HorizontalBox->AddChildToHorizontalBox(Image);
	ImageSlot->SetHorizontalAlignment(HAlign_Fill);
	ImageSlot->SetVerticalAlignment(VAlign_Fill);
	ImageSlot->SetSize(FSlateChildSize(ESlateSizeRule::Fill));
	
	UVerticalBox* VerticalBox = WidgetTree->ConstructWidget<UVerticalBox>(UVerticalBox::StaticClass());
	UHorizontalBoxSlot* VerticalBoxSlot = HorizontalBox->AddChildToHorizontalBox(VerticalBox);
	VerticalBoxSlot->SetHorizontalAlignment(HAlign_Fill);
	VerticalBoxSlot->SetVerticalAlignment(VAlign_Fill);
	VerticalBoxSlot->SetSize(FSlateChildSize(ESlateSizeRule::Fill));
	

	UTextBlock* Title = WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass());
	FSlateFontInfo TitleInfo = FCoreStyle::GetDefaultFontStyle("Regular", 26);
	Title->SetFont(TitleInfo); // 폰트 적용
	Title->SetText(FText::FromString(ItemData.ItemName));
	VerticalBox->AddChild(Title);

	UTextBlock* Desciption = WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass());
	FSlateFontInfo DesciptionInfo = FCoreStyle::GetDefaultFontStyle("Regular", 24);
	Desciption->SetFont(DesciptionInfo); // 폰트 적용
	Desciption->SetText(FText::FromString(ItemData.ItemName));
	VerticalBox->AddChild(Desciption);

	return Super::RebuildWidget();
}
