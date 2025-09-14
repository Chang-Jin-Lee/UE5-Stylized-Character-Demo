// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "LobbyHUD.generated.h"

/**
 * 
 */
UCLASS()
class CJSLATE_API ALobbyHUD : public AHUD
{
	GENERATED_BODY()
	
public:
	ALobbyHUD();
	virtual void BeginPlay() override;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "UI")
	TSubclassOf<class ULobbyPanelWidget> HUDPanelWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "UI")
	TObjectPtr<class ULobbyPanelWidget> HUDPannelWidget;
};
