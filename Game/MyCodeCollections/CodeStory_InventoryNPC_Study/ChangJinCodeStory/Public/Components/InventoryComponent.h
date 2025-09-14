// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Data/InventoryItem.h"
#include "InventoryComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class CHANGJINCODESTORY_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UInventoryComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	virtual void InitializeComponent() override;

public:    
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	TArray<FInventoryItem> Items;

	// Add an item to the inventory
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	bool AddItem(const FInventoryItem& NewItem);

	// Remove an item from the inventory by ID
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	bool RemoveItemByID(const FString& ItemID);

	// Find an item in the inventory by ID
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	int32 FindItemIndexByID(const FString& ItemID);

	// Update an existing item in the inventory by ID
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	bool UpdateItemByID(const FInventoryItem& UpdatedItem);
};
