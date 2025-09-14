// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Lena_InGameMenuPannel.generated.h"

/**
 * 
 */
UCLASS()
class LENA_API ULena_InGameMenuPannel : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual bool Initialize() override;
	
	UPROPERTY(meta=(BindWidgetOptional))
	TObjectPtr<UButton> Button_GoIntroPannel;

	UPROPERTY(meta=(BindWidgetOptional))
	TObjectPtr<UButton> Button_Restart;
	
	UPROPERTY(meta=(BindWidgetOptional))
	TObjectPtr<UButton> Button_Hint;

	UPROPERTY(meta=(BindWidgetOptional))
	TObjectPtr<UButton> Button_Guide;

	UPROPERTY(meta=(BindWidgetOptional))
	TObjectPtr<UButton> Button_Quit;

	UPROPERTY(meta=(BindWidgetOptional))
	TObjectPtr<UButton> Button_Exit;

	UFUNCTION()
	void OnGameRestart();
	UFUNCTION()
	void OnGameGoIntroPannel();
	UFUNCTION()
	void OnGameHint();
	UFUNCTION()
	void OnGameGuide();
	UFUNCTION()
	void OnGameQuit();
	UFUNCTION()
	void OnGameExit();
};
