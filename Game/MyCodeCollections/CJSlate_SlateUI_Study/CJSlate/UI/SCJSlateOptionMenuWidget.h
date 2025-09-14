// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class CJSLATE_API SCJSlateOptionMenuWidget : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SCJSlateOptionMenuWidget){}

		SLATE_ARGUMENT(TWeakObjectPtr<class ACJSlateHUD>, OwningHUD)
		
	SLATE_END_ARGS()
	SCJSlateOptionMenuWidget();
	~SCJSlateOptionMenuWidget();

	void Construct(const FArguments& args);

	FReply OnPlayButtonCliked();
	FReply OnBackToTitleButtonCliked();
	FReply OnCJSlateOptionButtonCliked();
	FReply OnExitButtonCliked();

	// 이 위젯이 만들어질 HUD
	TWeakObjectPtr<class ACJSlateHUD> OwningHUD;

	virtual bool SupportsKeyboardFocus() const override {return true;};
};
