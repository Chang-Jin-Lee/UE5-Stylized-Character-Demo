// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventoryUIWidget.generated.h"

class UBase_ItemSlotWidget;
class ABase_Character;
/**
 * 
 */
UCLASS()
class CHANGJINCODESTORY_API UInventoryUIWidget : public UUserWidget
{
	GENERATED_BODY()

	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;;
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	int32 Columns;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	int32 Rows;

	virtual TSharedRef<SWidget> RebuildWidget() override;

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void InitializeInventory();

	// UFUNCTION(BlueprintCallable, Category = "Inventory")
	// void AddItemToSlot(int32 Column, int32 Row, class UItemWidget* Item);

	ABase_Character* Base_Character;

private:
	class UUniformGridPanel* InventoryGrid;

	UBase_ItemSlotWidget* GetItemSlotWidgetFromParent(UWidget* ParentWidget);
	UWidget* GetUWidgetFromPosition(FVector2D Position);
};
