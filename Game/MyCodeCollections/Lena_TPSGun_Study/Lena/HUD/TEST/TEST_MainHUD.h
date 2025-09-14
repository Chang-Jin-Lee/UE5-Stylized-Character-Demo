// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "TEST_MainHUD.generated.h"

/**
 * 
 */
class UTEST_MainPanelWidget;

UCLASS()
class LENA_API ATEST_MainHUD : public AHUD
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;
	TObjectPtr<UTEST_MainPanelWidget> WidgetPanel;
};
