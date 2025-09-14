// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CJSlate/Interface/GuideWidgetInterface.h"
#include "InGamePanelWidget.generated.h"

class UInventoryWidget;
class UCanvasPanel;
class UBorder;
class UImage;
class UInterationWidget;
class UTextBlock;
/**
 * 
 */
UCLASS()
class CJSLATE_API UInGamePanelWidget : public UUserWidget, public IGuideWidgetInterface
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	
	UFUNCTION()
	void Interact(FString ObjectName);
	
	UFUNCTION()
	void InteractDone();

	UPROPERTY(meta = (BindWidgetOptional))
	UImage* CrosshairImage;

	UPROPERTY(meta = (BindWidgetOptional))
	UInterationWidget* InteractionWidget;

	UPROPERTY(meta = (BindWidgetOptional))
	UInventoryWidget* InventoryWidget;

	virtual void PopUp(FText text) override;
	UFUNCTION()
	virtual void PopDown() override;

	// GuideWidget 관련
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

	// 애니메이션을 실행하기전에 Text를 바꾸고 실행해야함
	void SetText_TextBlock_Guide(FText _value);
	UFUNCTION()
	virtual void PlayAnimation_GuideFadeIn() override;
	UFUNCTION()
	virtual void PlayAnimation_GuideFadeOut() override;

	virtual void TimeOut() override;

	FTimerHandle TimerHandle;
};
