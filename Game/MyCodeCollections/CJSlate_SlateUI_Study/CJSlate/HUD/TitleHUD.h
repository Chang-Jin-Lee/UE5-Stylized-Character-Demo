// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "TitleHUD.generated.h"

/**
 * 
 */
UCLASS()
class CJSLATE_API ATitleHUD : public AHUD
{
	GENERATED_BODY()
	
public:
	ATitleHUD();
	virtual void BeginPlay() override;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "UI")
	TSubclassOf<class UTitleScreenUI> HUDPanelWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "UI")
	TObjectPtr<class UTitleScreenUI> HUDPanelWidget;
};
