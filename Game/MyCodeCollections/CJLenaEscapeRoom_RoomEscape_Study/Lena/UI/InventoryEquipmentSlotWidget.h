// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InventorySlotWidget.h"
#include "Components/Image.h"
#include "InventoryEquipmentSlotWidget.generated.h"

/**
 * 
 */
UCLASS()
class LENA_API UInventoryEquipmentSlotWidget : public UInventorySlotWidget
{
	GENERATED_BODY()

public:
	FORCEINLINE void SetEquipmentSlotIndex(int32 _index){ EquipmentSlotIndex = _index; }

	void LoadData();
	
	UPROPERTY(meta=(BindWidgetOptional))
	TObjectPtr<UTextBlock> TextBlock_ItemName;
	
	UPROPERTY(meta=(BindWidgetOptional))
	TObjectPtr<UTextBlock> TextBlock_ItemQuantiy;

	UPROPERTY(meta=(BindWidgetOptional))
	TObjectPtr<UTextBlock> TextBlock_ItemIndex;
	
	UPROPERTY(meta=(BindWidgetOptional))
	TObjectPtr<UImage> Image_ItemImage;

	void SetItemName();
	void SetItemQuantiy();
	void SetImage();
	void SetIndex();

private:
	int32 EquipmentSlotIndex;
};
