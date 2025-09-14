// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventoryWidget.generated.h"

class UCanvasPanel;
class UUniformGridPanel;
class UInventoryWidgetSlot;
/**
 * 
 */
UCLASS()
class CJSLATE_API UInventoryWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct() override;
	
	void LoadData();
	
	UPROPERTY(meta = (BindWidgetOptional))
	UUniformGridPanel* UniformGridPanel_Item;

	UPROPERTY(EditAnywhere)
	int32 ItemRow = 5;
	
	UPROPERTY(EditAnywhere)
	int32 ItemCol = 3;

	// 그리드 패널에 그릴 슬롯
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Inventory")
	TSubclassOf<UInventoryWidgetSlot> UInventoryWidgetSlotClass;

	UPROPERTY()
	TArray<UInventoryWidgetSlot*> InventoryWidgetSlots;
};
