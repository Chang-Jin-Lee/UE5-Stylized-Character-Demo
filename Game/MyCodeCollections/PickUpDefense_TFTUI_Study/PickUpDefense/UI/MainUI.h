// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RoundResultPanel.h"
#include "Blueprint/UserWidget.h"
#include "MainUI.generated.h"

class UShopPanel;
/**
 * 
 */
UCLASS()
class PICKUPDEFENSE_API UMainUI : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual TSharedRef<SWidget> RebuildWidget() override;

private:
	UPROPERTY()
	URoundResultPanel* RoundResultPanel;

	UPROPERTY()
	UShopPanel* ShopPanel;
};
