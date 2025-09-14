// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "InventoryDataTableManager.generated.h"

struct FGameplayTagContainer;
struct FInventoryItemData;
struct FGameplayTag;
/**
 * 
 */
UCLASS()
class CJSLATE_API UInventoryDataTableManager : public UObject
{
	GENERATED_BODY()
public:
	UInventoryDataTableManager();
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "DataTable")
	UDataTable* InventoryDataTable;

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	bool IsItemTagPresent(const FGameplayTag& ItemTag) const;

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	TArray<FInventoryItemData> GetItemsByTag(const FGameplayTag& ItemTag) const;

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	TArray<FInventoryItemData> GetMatchingItemsByTagAndName(const FGameplayTagContainer& ItemTags, const FString& ItemName) const;
	
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	FInventoryItemData GetMatchingItemByTagAndName(const FGameplayTagContainer& ItemTags, const FString& ItemName) const;
};
