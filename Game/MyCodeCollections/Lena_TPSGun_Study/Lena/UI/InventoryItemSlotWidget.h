// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InventorySlotWidget.h"
#include "InventoryItemSlotWidget.generated.h"

/**
 * 
 */
UCLASS()
class LENA_API UInventoryItemSlotWidget : public UInventorySlotWidget
{
	GENERATED_BODY()

public:
	virtual bool Initialize() override;
	void LoadData();
	
	UPROPERTY(meta=(BindWidgetOptional))
	TObjectPtr<UTextBlock> TextBlock_ItemName;
	
	UPROPERTY(meta=(BindWidgetOptional))
	TObjectPtr<UTextBlock> TextBlock_ItemQuantity;

	void SetItemName();
	void SetItemQuantiy();
};
