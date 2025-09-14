// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "NameTagWidget.generated.h"

class UTextBlock;
/**
 * 
 */
UCLASS()
class CHANGJINCODESTORY_API UNameTagWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UTextBlock* NameTextBlock;
	
protected:
	virtual TSharedRef<SWidget> RebuildWidget() override;
};
