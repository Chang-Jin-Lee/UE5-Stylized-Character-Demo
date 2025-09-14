// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "VoteWidget.generated.h"

class UButton;
/**
 * 
 */
UCLASS()
class CJSLATE_API UVoteWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	UPROPERTY(meta = (BindWidgetOptional))
	UButton* Button_Yes;
	
	UPROPERTY(meta = (BindWidgetOptional))
	UButton* Button_No;

	UFUNCTION()
	void Button_Yes_Clicked();
	UFUNCTION()
	void Button_No_Clicked();
};
