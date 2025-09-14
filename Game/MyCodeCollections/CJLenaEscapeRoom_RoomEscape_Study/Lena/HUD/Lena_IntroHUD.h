// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "Lena_IntroHUD.generated.h"

class ULena_IntroPannelWidget;
/**
 * 
 */
UCLASS()
class LENA_API ALena_IntroHUD : public AHUD
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;
	TObjectPtr<ULena_IntroPannelWidget> WidgetPanel;
};
