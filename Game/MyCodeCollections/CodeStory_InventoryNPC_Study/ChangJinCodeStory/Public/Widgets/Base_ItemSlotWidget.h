// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Characters/Base_Character.h"
#include "Data/InventoryItem.h"
#include "Base_ItemSlotWidget.generated.h"

class UInventoryHoverWidget;
class UImage;
/**
 * 
 */
UCLASS()
class CHANGJINCODESTORY_API UBase_ItemSlotWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual TSharedRef<SWidget> RebuildWidget() override;

	void SetItemImage(UTexture2D* NewImage);
	
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, meta = (BindWidget))
	UImage* Image;

	virtual FReply NativeOnPreviewMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation) override;
	virtual void NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override;
	virtual void NativeDestruct() override;

	virtual FReply NativeOnMouseMove(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	
	FInventoryItem Itemdata;
	ABase_Character* Base_Character;
	int32 SlotIndex;
	UInventoryHoverWidget* HoverWidget;
};