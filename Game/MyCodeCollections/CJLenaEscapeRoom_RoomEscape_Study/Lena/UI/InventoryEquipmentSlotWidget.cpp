// Fill out your copyright notice in the Description page of Project Settings.

#include "InventoryEquipmentSlotWidget.h"

#include "Components/TextBlock.h"

void UInventoryEquipmentSlotWidget::LoadData()
{
	SetItemName();
	SetItemQuantiy();
	SetImage();
	SetIndex();
}

void UInventoryEquipmentSlotWidget::SetItemName()
{
	if(TextBlock_ItemName)
		TextBlock_ItemName->SetText(FText::FromString(ItemData.ItemName));
}

void UInventoryEquipmentSlotWidget::SetItemQuantiy()
{
	if(TextBlock_ItemQuantiy)
	{
		FString _text = FString::SanitizeFloat(ItemData.Quantity);
		TextBlock_ItemQuantiy->SetText(FText::FromString(_text));
	}
}

void UInventoryEquipmentSlotWidget::SetImage()
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

void UInventoryEquipmentSlotWidget::SetIndex()
{
	if(TextBlock_ItemIndex)
	{
		FString _text = FString::FromInt(EquipmentSlotIndex);
		TextBlock_ItemIndex->SetText(FText::FromString(_text));
	}
}
