// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Data/InventoryItem.h"
#include "InventoryHoverWidget.generated.h"

/**
 * 
 */
UCLASS()
class CHANGJINCODESTORY_API UInventoryHoverWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual TSharedRef<SWidget> RebuildWidget() override;

	FInventoryItem ItemData;
};
