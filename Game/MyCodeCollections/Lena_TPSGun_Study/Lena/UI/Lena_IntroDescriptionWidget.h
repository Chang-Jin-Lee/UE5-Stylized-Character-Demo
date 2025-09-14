// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Lena_IntroPannelWidget.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Lena_IntroDescriptionWidget.generated.h"

/**
 * 
 */
UCLASS()
class LENA_API ULena_IntroDescriptionWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual bool Initialize() override;
	UFUNCTION()
	void OnRoom01();
	UFUNCTION()
	void OnRoom02();
	UFUNCTION()
	void OnRoom03();
	UFUNCTION()
	void OnGoIntro();

	UPROPERTY(meta=(BindWidgetOptional))
	TObjectPtr<UButton> Button_Room01;

	UPROPERTY(meta=(BindWidgetOptional))
	TObjectPtr<UButton> Button_Room02;

	UPROPERTY(meta=(BindWidgetOptional))
	TObjectPtr<UButton> Button_Room03;

	UPROPERTY(meta=(BindWidgetOptional))
	TObjectPtr<UButton> Button_GoIntroWidget;

	FORCEINLINE void SetInroPanel(ULena_IntroPannelWidget* _intropannel) {IntroPannel = _intropannel;}

private:
	UPROPERTY()
	TObjectPtr<ULena_IntroPannelWidget> IntroPannel;
};
