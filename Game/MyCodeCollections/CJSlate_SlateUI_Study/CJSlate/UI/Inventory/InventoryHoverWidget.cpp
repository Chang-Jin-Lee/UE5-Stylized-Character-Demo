// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryHoverWidget.h"

#include "Components/Image.h"
#include "Components/TextBlock.h"

void UInventoryHoverWidget::Load(FItemActorInfo ItemActorInfo)
{
	TextBlock_ItemName->SetText(FText::FromString(ItemActorInfo.ItemName));
	Image_ItemImage->SetBrushFromTexture(ItemActorInfo.ItemImage);
	TextBlock_ItemDescription->SetText(ItemActorInfo.ItemDescription);
	TextBlock_ItemCount->SetText(FText::FromString(FString::FromInt(ItemActorInfo.ItemCount)));
}
