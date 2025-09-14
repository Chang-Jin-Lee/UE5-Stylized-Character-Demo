// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InventoryHoverWidget.h"
#include "InventoryWidget.h"
#include "Lena/Characters/Base_Character.h"
#include "Lena/Data//InventoryItem.h"

#include "InventorySlotWidget.generated.h"

/**
 * 
 */
UCLASS()
class LENA_API UInventorySlotWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual FReply NativeOnPreviewMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation) override;
	virtual void NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	void MovdHoverWidgetPosition();
	virtual FReply NativeOnMouseMove(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override;
public:
	FORCEINLINE FInventoryItem GetItemData() { return ItemData; }
	FORCEINLINE void SetItemData(FInventoryItem _item) { ItemData = _item; }
	FORCEINLINE void SetBaseCharacter(TObjectPtr<ABase_Character> _ch) { Base_Character = _ch;}
	FORCEINLINE TObjectPtr<ABase_Character> GetBaseCharacter() { return Base_Character;}
	FORCEINLINE void SetHoverWidget(TObjectPtr<UInventoryHoverWidget> _widget) { HoverWidget = _widget;}
	FORCEINLINE TObjectPtr<UInventoryHoverWidget> GetHoverWidget() { return HoverWidget ;}
	FORCEINLINE void SetInventoryWidgetMain(TObjectPtr<UInventoryWidget> _widget) { InventoryWidgetMain = _widget;}
	
protected:
	FInventoryItem ItemData;
	UPROPERTY()
	TObjectPtr<ABase_Character> Base_Character;
	FInventoryItem DroppedItem;
	UPROPERTY()
	TObjectPtr<UInventoryHoverWidget> HoverWidget;
	UPROPERTY()
	TObjectPtr<UInventoryWidget> InventoryWidgetMain;
};
