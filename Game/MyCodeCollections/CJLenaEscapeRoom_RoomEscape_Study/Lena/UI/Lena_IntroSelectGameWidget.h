// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Lena_IntroPannelWidget.h"
#include "Components/Button.h"
#include "Lena_IntroSelectGameWidget.generated.h"

UCLASS()
class LENA_API ULena_IntroSelectGameWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual bool Initialize() override;
	UFUNCTION()
	void OnRoom01Animation();
	UFUNCTION()
	void OnRoom02Animation();
	UFUNCTION()
	void OnRoom03Animation();
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

	UPROPERTY(meta=(BindWidgetAnimOptional), Transient)
	TObjectPtr<UWidgetAnimation> FadeOut;

	UPROPERTY(meta=(BindWidgetAnimOptional), Transient)
	TObjectPtr<UWidgetAnimation> Animation_OnClickRoom01;

	UPROPERTY(meta=(BindWidgetAnimOptional), Transient)
	TObjectPtr<UWidgetAnimation> Animation_OnClickRoom02;

	UPROPERTY(meta=(BindWidgetAnimOptional), Transient)
	TObjectPtr<UWidgetAnimation> Animation_OnClickRoom03;

	FORCEINLINE void SetInroPanel(TObjectPtr<ULena_IntroPannelWidget> _intropannel) {IntroPannel = _intropannel;}
	
private:
	UPROPERTY()
	TObjectPtr<ULena_IntroPannelWidget> IntroPannel;
	FTimerHandle Timer;
};
