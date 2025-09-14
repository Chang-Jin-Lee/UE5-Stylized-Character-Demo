// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Lena_IntroPannelWidget.generated.h"

UCLASS()
class LENA_API ULena_IntroPannelWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual bool Initialize() override;
	UFUNCTION()
	void OnGameStartTime();
	UFUNCTION()
	void OnGameStart();
	UFUNCTION()
	void OnClickDesciptionTime();
	UFUNCTION()
	void OnClickDesciption();
	UFUNCTION()
	void OnQuitTime();
	UFUNCTION()
	void OnQuit();
	void PlayFadeIn();

	UPROPERTY(meta=(BindWidgetOptional))
	TObjectPtr<UButton> Button_GameStart;

	UPROPERTY(meta=(BindWidgetOptional))
	TObjectPtr<UButton> Button_Description;

	UPROPERTY(meta=(BindWidgetOptional))
	TObjectPtr<UButton> Button_Quit;

	UPROPERTY(meta=(BindWidgetAnimOptional), Transient)
	TObjectPtr<UWidgetAnimation> FadeOut;

	UPROPERTY(meta=(BindWidgetAnimOptional), Transient)
	TObjectPtr<UWidgetAnimation> FadeIn;

private:
	FTimerHandle Timer;
};
