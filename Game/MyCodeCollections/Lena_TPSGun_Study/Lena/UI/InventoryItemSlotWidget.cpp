// Fill out your copyright notice in the Description page of Project Settings.

#include "InventoryItemSlotWidget.h"

#include "Components/TextBlock.h"

bool UInventoryItemSlotWidget::Initialize()
{
	return Super::Initialize();
}

void UInventoryItemSlotWidget::LoadData()
{
	SetItemName();
	SetItemQuantiy();
}

void UInventoryItemSlotWidget::SetItemName()
{
	if(TextBlock_ItemName)
		TextBlock_ItemName->SetText(FText::FromString(ItemData.ItemName));
}

void UInventoryItemSlotWidget::SetItemQuantiy()
{
	if(TextBlock_ItemQuantity)
	{
		FString _text = FString::SanitizeFloat(ItemData.Quantity);
		TextBlock_ItemQuantity->SetText(FText::FromString(_text));
	}
}
 