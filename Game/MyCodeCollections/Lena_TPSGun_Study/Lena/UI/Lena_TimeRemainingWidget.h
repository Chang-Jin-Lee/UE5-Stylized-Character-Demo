// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Lena_TimeRemainingWidget.generated.h"

class UTextBlock;
/**
 * 
 */
UCLASS()
class LENA_API ULena_TimeRemainingWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<UTextBlock> TextBlock_Description;

	void SetDescriptionText(FString _string);
	void VisibleDescriptionText();
	void HiddenDescriptionText();
};
