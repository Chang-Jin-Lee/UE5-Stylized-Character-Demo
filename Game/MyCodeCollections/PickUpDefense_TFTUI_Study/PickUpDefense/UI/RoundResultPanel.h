// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RoundResultPanel.generated.h"

/**
 * 
 */
UCLASS()
class PICKUPDEFENSE_API URoundResultPanel : public UUserWidget
{
	GENERATED_BODY()
	
	virtual TSharedRef<SWidget> RebuildWidget() override;
};
