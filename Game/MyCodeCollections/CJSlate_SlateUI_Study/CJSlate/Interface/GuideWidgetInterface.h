// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/TimelineComponent.h"
#include "UObject/Interface.h"

#include "GuideWidgetInterface.generated.h"

class UBorder;
class UWidgetAnimation;
class UTextBlock;
class UCircularThrobber;

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UGuideWidgetInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 쓰이고 있는 Widget 목록
 * - TitleScreenUI, LobbyPannelWidget, UHUDPannel
 * CircularThrobber : 기다린다는 표시를 해줄 로딩 바
 * TextBlockDescription : 어떤 행동을 했는지 보여 줄 텍스트
 */
class CJSLATE_API IGuideWidgetInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	
	/** 다음의 내용을 복사 붙여넣기 할것.
	*	UPROPERTY(meta = (BindWidgetOptional))
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
	 * @return 
	 */
	virtual UCircularThrobber* GetLoadingBar() const = 0;
	virtual UTextBlock* GetDescriptionText() const = 0;
	virtual UBorder* GetGuideBackGroundBorder() const = 0;
	virtual UWidgetAnimation* GetGuideFadeInAnimation() const = 0;
	virtual UWidgetAnimation* GetGuideFadeOutAnimation() const = 0;
	
	UFUNCTION()
	virtual void PopUp(FText text);

	UFUNCTION()
	virtual void PopDown();

	// 애니메이션을 실행시킬때 쓸 타이머.
	// ex) 애니메이션 실행 후 몇 초후에 창이 닫힐지.
	FTimerHandle GuideWidgetTimerHandle;

	// Fade in , Fade Out 할때 중간에 Fade out 이전에 텍스트를 띄우기 위함
	FTimerHandle GuideWidgetTimerHandleFadeOut;

	UFUNCTION()
	virtual void PlayAnimation_GuideFadeIn() = 0;

	UFUNCTION()
	virtual void PlayAnimation_GuideFadeOut() = 0;

	UFUNCTION()
	virtual void TimeOut() = 0;

	float TimeOutValue = 12.0f;
};
