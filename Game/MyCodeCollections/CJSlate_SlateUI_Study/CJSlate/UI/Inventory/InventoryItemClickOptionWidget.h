// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CJSlate/Item/ItemActor.h"
#include "InventoryItemClickOptionWidget.generated.h"

class UInventoryWidgetSlot;
class UButton;
/**
 * 
 */
UCLASS()
class CJSLATE_API UInventoryItemClickOptionWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct() override;

	// InventoryWidgetSlot으로부터 전달 받은 아이템 정보.
	UPROPERTY()
	FItemActorInfo ItemActorInfo;

	UPROPERTY()
	UInventoryWidgetSlot* ParentSlot;
	
	UPROPERTY(meta = (BindWidgetOptional))
	UButton* Button_Drop;
	
	UPROPERTY(meta = (BindWidgetOptional))
	UButton* Button_Close;

	UFUNCTION()
	void Drop();
	
	UFUNCTION()
	void Close();
};
