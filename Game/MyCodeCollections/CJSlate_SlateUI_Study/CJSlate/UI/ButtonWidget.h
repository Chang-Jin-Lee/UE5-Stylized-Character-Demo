// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ButtonWidget.generated.h"

/**
 * 
 */
UCLASS()
class CJSLATE_API UButtonWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(meta = (BindWidgetOptional))
	class UTextBlock* TextBlock;

	void SetTextBlock(FString text);
};
