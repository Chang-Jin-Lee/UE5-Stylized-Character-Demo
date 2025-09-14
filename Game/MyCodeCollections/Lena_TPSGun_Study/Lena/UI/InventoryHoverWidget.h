// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Lena/Data/InventoryItem.h"
#include "InventoryHoverWidget.generated.h"

/**
 * 
 */
UCLASS()
class LENA_API UInventoryHoverWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	FORCEINLINE void SetItemData(FInventoryItem _itemData) { ItemData = _itemData; }

	UPROPERTY(meta=(BindWidgetAnimOptional), Transient)
	TObjectPtr<UWidgetAnimation> WidgetAnimation_FadeIn;

	UPROPERTY(meta=(BindWidgetAnimOptional), Transient)
	TObjectPtr<UWidgetAnimation> WidgetAnimation_FadeOut;

	void PlayFadeIn();
	void PlayFadeOut();
	void RemoveFromParentTimer();
	
	UPROPERTY(meta=(BindWidgetOptional))
	TObjectPtr<UTextBlock> TextBlock_ItemName;

	UPROPERTY(meta=(BindWidgetOptional))
	TObjectPtr<UImage> Image_ItemImage;

	UPROPERTY(meta=(BindWidgetOptional))
	TObjectPtr<UTextBlock> TextBlock_ItemDescription;

	UPROPERTY(meta=(BindWidgetOptional))
	TObjectPtr<UTextBlock> TextBlock_ItemWeight;

	void LoadData();
	void SetItemName();
	void SetItemImage();
	void SetItemDesciption();
	void SetItemWeight();
	
private:
	FInventoryItem ItemData;
	FTimerHandle FadeOutTimer;
};