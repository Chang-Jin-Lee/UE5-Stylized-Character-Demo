// Fill out your copyright notice in the Description page of Project Settings.

#include "InventoryGroundSlotWidget.h"

#include "Components/TextBlock.h"


bool UInventoryGroundSlotWidget::Initialize()
{
	return Super::Initialize();
}

void UInventoryGroundSlotWidget::LoadData()
{
	SetItemName();
	SetItemQuantiy();
}

void UInventoryGroundSlotWidget::SetItemName()
{
	if(TextBlock_ItemName)
		TextBlock_ItemName->SetText(FText::FromString(ItemData.ItemName));
}

void UInventoryGroundSlotWidget::SetItemQuantiy()
{
	if(TextBlock_ItemQuantiy)
	{
		FString _text = FString::SanitizeFloat(ItemData.Quantity);
		TextBlock_ItemQuantiy->SetText(FText::FromString(_text));
	}
}
