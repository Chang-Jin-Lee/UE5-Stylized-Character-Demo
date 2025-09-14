// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryDataTableManager.h"

#include "InventoryData.h"

UInventoryDataTableManager::UInventoryDataTableManager()
{
	static ConstructorHelpers::FObjectFinder<UDataTable> DataTableAsset(TEXT("/Game/DataTable/InventoryItemData.InventoryItemData"));
	if (DataTableAsset.Succeeded())
	{
		InventoryDataTable = DataTableAsset.Object;
	}
}

bool UInventoryDataTableManager::IsItemTagPresent(const FGameplayTag& ItemTag) const
{
	if (!InventoryDataTable || InventoryDataTable->RowStruct != FInventoryItemData::StaticStruct()) return false;

	TArray<FName> RowNames = InventoryDataTable->GetRowNames();
	for (const FName& RowName : RowNames)
	{
		FInventoryItemData* Row = InventoryDataTable->FindRow<FInventoryItemData>(RowName, TEXT("Item Lookup"));
		if (Row && Row->ItemTags.HasTag(ItemTag))
		{
			return true;
		}
	}

	return false;
}


TArray<FInventoryItemData> UInventoryDataTableManager::GetItemsByTag(const FGameplayTag& ItemTag) const
{
	TArray<FInventoryItemData> MatchingItems;
	if (!InventoryDataTable) return MatchingItems;

	TArray<FInventoryItemData*> AllRows;
	InventoryDataTable->GetAllRows<FInventoryItemData>(TEXT("Item Lookup"), AllRows);

	for (const FInventoryItemData* Row : AllRows)
	{
		if (Row->ItemTags.HasTag(ItemTag))
		{
			MatchingItems.Add(*Row);
		}
	}

	return MatchingItems;
}

TArray<FInventoryItemData> UInventoryDataTableManager::GetMatchingItemsByTagAndName(const FGameplayTagContainer& ItemTags, const FString& ItemName) const
{
	TArray<FInventoryItemData> MatchingItems;
	if (!InventoryDataTable || InventoryDataTable->RowStruct != FInventoryItemData::StaticStruct()) return MatchingItems;

	TArray<FName> RowNames = InventoryDataTable->GetRowNames();
	for (const FName& RowName : RowNames)
	{
		FInventoryItemData* Row = InventoryDataTable->FindRow<FInventoryItemData>(RowName, TEXT("Item Lookup"));
		if (Row && Row->ItemTags.HasAllExact(ItemTags) && Row->ItemName.Equals(ItemName, ESearchCase::IgnoreCase))
		{
			MatchingItems.Add(*Row);
		}
	}

	return MatchingItems;
}

FInventoryItemData UInventoryDataTableManager::GetMatchingItemByTagAndName(
	const FGameplayTagContainer& ItemTags, const FString& ItemName) const
{
	if (!InventoryDataTable || InventoryDataTable->RowStruct != FInventoryItemData::StaticStruct()) return FInventoryItemData();

	TArray<FName> RowNames = InventoryDataTable->GetRowNames();
	for (const FName& RowName : RowNames)
	{
		FInventoryItemData* Row = InventoryDataTable->FindRow<FInventoryItemData>(RowName, TEXT("Item Lookup"));
		if (Row && Row->ItemTags.HasAllExact(ItemTags) && Row->ItemName.Equals(ItemName, ESearchCase::IgnoreCase))
		{
			return *Row;
		}
	}

	return FInventoryItemData();
}
