// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/ItemWidget.h"

#include "Blueprint/WidgetTree.h"
#include "Components/Image.h"

TSharedRef<SWidget> UItemWidget::RebuildWidget()
{
	Super::RebuildWidget();

	// 이미지 위젯 생성
	ItemImage = WidgetTree->ConstructWidget<UImage>(UImage::StaticClass(), TEXT("ItemImage"));

	if (ItemIcon)
	{
		ItemImage->SetBrushFromTexture(ItemIcon);
	}
	else
	{
		FSlateBrush Brush;
		Brush.TintColor = FSlateColor(FLinearColor::Transparent);
		ItemImage->SetBrush(Brush);
	}

	WidgetTree->RootWidget = ItemImage;

	return Super::RebuildWidget();
}
