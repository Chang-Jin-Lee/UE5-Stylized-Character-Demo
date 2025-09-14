// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CJSlate/Interface/GuideWidgetInterface.h"
#include "TitleScreenUI.generated.h"

class UBorder;
class UCircularThrobber;
class UTextBlock;
class UEditableText;
/**
 * 
 */
class UButton;

UCLASS()
class CJSLATE_API UTitleScreenUI : public UUserWidget, public IGuideWidgetInterface
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

	UPROPERTY(meta = (BindWidgetOptional))
	UButton* ConnectServerButton;

	UPROPERTY(meta = (BindWidgetOptional))
	UButton* QuitButton;

	UPROPERTY(meta = (BindWidgetOptional))
	UEditableText* EditTextIpAdress;

	UPROPERTY(meta = (BindWidgetAnimOptional), Transient)
	UWidgetAnimation* GuideFadein;

	UPROPERTY(meta = (BindWidgetAnimOptional), Transient)
	UWidgetAnimation* GuideFadeOut;
	
	UFUNCTION()
	void ConnectServer();
	
	UFUNCTION()
	void Quit();

	UFUNCTION()
	void InputText(const FText& _value);

	UFUNCTION()
	void InputTextCommited(const FText& _value, ETextCommit::Type _commited);

	UPROPERTY()
	FText IpAdress;

	// IGuideWidgetInterface
	UPROPERTY(meta = (BindWidgetOptional))
	UCircularThrobber* CircularThrobber_LoadingBar;

	UPROPERTY(meta = (BindWidgetOptional))
	UTextBlock* TextBlock_Description;
	
	UPROPERTY(meta = (BindWidgetOptional))
	UBorder* Border_GuideBackGround;

	UPROPERTY(meta = (BindWidgetAnimOptional), Transient)
	UWidgetAnimation* WidgetAnimation_GuideFadein;

	UPROPERTY(meta = (BindWidgetAnimOptional), Transient)
	UWidgetAnimation* WidgetAnimation_GuideFadeOut;

	virtual UCircularThrobber* GetLoadingBar() const override { return CircularThrobber_LoadingBar; }
	virtual UTextBlock* GetDescriptionText() const override { return TextBlock_Description; }
	virtual UBorder* GetGuideBackGroundBorder() const override { return Border_GuideBackGround; }
	virtual UWidgetAnimation* GetGuideFadeInAnimation() const override { return WidgetAnimation_GuideFadein; }
	virtual UWidgetAnimation* GetGuideFadeOutAnimation() const override { return WidgetAnimation_GuideFadeOut; }

	UFUNCTION()
	virtual void PlayAnimation_GuideFadeIn() override;

	UFUNCTION()
	virtual void PlayAnimation_GuideFadeOut() override;

	UFUNCTION()
	virtual void TimeOut() override;
};