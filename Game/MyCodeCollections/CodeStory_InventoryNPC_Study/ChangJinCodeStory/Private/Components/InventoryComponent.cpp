// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/InventoryComponent.h"



UInventoryComponent::UInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UInventoryComponent::InitializeComponent()
{
	Super::InitializeComponent();
	Items.Empty();
}

void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();
}

bool UInventoryComponent::AddItem(const FInventoryItem& NewItem)
{
	// Check if the item already exists in the inventory by ID
	int32 ItemIndex = FindItemIndexByID(NewItem.ItemID);
	if (ItemIndex != INDEX_NONE)
	{
		// If the item already exists, increase its quantity
		Items[ItemIndex].Quantity += NewItem.Quantity;
		return true;
	}
	
	// If the item doesn't exist, add it to the inventory
	Items.Add(NewItem);
	return true;
}

bool UInventoryComponent::RemoveItemByID(const FString& ItemID)
{
	for (int32 i = 0; i < Items.Num(); i++)
	{
		if (Items[i].ItemID == ItemID)
		{
			Items.RemoveAt(i);
			return true;
		}
	}
	return false; // Item not found
}

int32 UInventoryComponent::FindItemIndexByID(const FString& ItemID)
{
	for (int32 Index = 0; Index < Items.Num(); Index++)
	{
		if (Items[Index].ItemID == ItemID)
		{
			return Index;
		}
	}
	return INDEX_NONE; // 아이템이 없을 경우, -1을 반환
}

bool UInventoryComponent::UpdateItemByID(const FInventoryItem& UpdatedItem)
{
	int32 ItemIndex = FindItemIndexByID(UpdatedItem.ItemID);
	if (ItemIndex != INDEX_NONE)
	{
		// Update the existing item with new values
		Items[ItemIndex] = UpdatedItem;
		return true;
	}
	return false; // Item not found
}