// Fill out your copyright notice in the Description page of Project Settings.

#include "InventoryHoverWidget.h"

#include "TimerManager.h"

void UInventoryHoverWidget::PlayFadeIn()
{
	if(WidgetAnimation_FadeIn)
		PlayAnimation(WidgetAnimation_FadeIn, 0, 1);
}

void UInventoryHoverWidget::PlayFadeOut()
{
	if(WidgetAnimation_FadeOut)
	{
		PlayAnimation(WidgetAnimation_FadeOut, 0 ,1);
		GetWorld()->GetTimerManager().SetTimer(FadeOutTimer, this, &UInventoryHoverWidget::RemoveFromParentTimer, 0.5f, false);
	}
}

void UInventoryHoverWidget::RemoveFromParentTimer()
{
	RemoveFromParent();
}

void UInventoryHoverWidget::LoadData()
{
	SetItemDesciption();
	SetItemImage();
	SetItemName();
	SetItemWeight();
}

void UInventoryHoverWidget::SetItemName()
{
	if(TextBlock_ItemName)
		TextBlock_ItemName->SetText(FText::FromString(ItemData.ItemName));
}

void UInventoryHoverWidget::SetItemImage()
{
	UTexture2D* texture = ItemData.ItemImage;
	if(texture)
	{
		Image_ItemImage->SetColorAndOpacity(FLinearColor::White);
		Image_ItemImage->SetBrushFromTexture(texture);
	}
	else
	{
		Image_ItemImage->SetColorAndOpacity(FLinearColor::Transparent);
	}
}

void UInventoryHoverWidget::SetItemDesciption()
{
	if(TextBlock_ItemDescription)
		TextBlock_ItemDescription->SetText(FText::FromString(ItemData.ItemDescription));
}

void UInventoryHoverWidget::SetItemWeight()
{
	if(TextBlock_ItemWeight)
	{
		FString _text = FString::Printf(TEXT("weight : %.2f Kg"), ItemData.Weight);
		TextBlock_ItemWeight->SetText(FText::FromString(_text));
	}
}
