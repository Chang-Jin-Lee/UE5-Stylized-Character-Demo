// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ItemWidget.generated.h"

/**
 * 
 */
UCLASS()
class CHANGJINCODESTORY_API UItemWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	class UTexture2D* ItemIcon;

	UPROPERTY(meta = (BindWidget))
	class UImage* ItemImage;

	virtual TSharedRef<SWidget> RebuildWidget() override;
};