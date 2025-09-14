// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CJSlate/Item/ItemActor.h"
#include "InventoryHoverWidget.generated.h"

class UImage;
class UTextBlock;
/**
 * 
 */
UCLASS()
class CJSLATE_API UInventoryHoverWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void Load(FItemActorInfo ItemActorInfo);
	
	UPROPERTY(meta = (BindWidgetOptional))
	UTextBlock* TextBlock_ItemName;
	
	UPROPERTY(meta = (BindWidgetOptional))
	UImage* Image_ItemImage;

	UPROPERTY(meta = (BindWidgetOptional))
	UTextBlock* TextBlock_ItemDescription;

	UPROPERTY(meta = (BindWidgetOptional))
	UTextBlock* TextBlock_ItemCount;
};
