// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InventorySlotWidget.h"
#include "InventoryGroundSlotWidget.generated.h"

/**
 * 
 */
UCLASS()
class LENA_API UInventoryGroundSlotWidget : public UInventorySlotWidget
{
	GENERATED_BODY()

		
	virtual bool Initialize() override;
	
public:
	void LoadData();
	
	UPROPERTY(meta=(BindWidgetOptional))
	TObjectPtr<UTextBlock> TextBlock_ItemName;
	
	UPROPERTY(meta=(BindWidgetOptional))
	TObjectPtr<UTextBlock> TextBlock_ItemQuantiy;

	void SetItemName();
	void SetItemQuantiy();
};
