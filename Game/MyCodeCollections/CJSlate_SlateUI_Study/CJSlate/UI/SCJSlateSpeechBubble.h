// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CJSlate/CJSlateCharacter.h"
#include "Widgets/SCompoundWidget.h"

/**
 * 
 */
class CJSLATE_API SCJSlateSpeechBubble : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SCJSlateSpeechBubble) {}
		SLATE_ARGUMENT(FText, Message)
		SLATE_ARGUMENT(FSlateColor, BackgroundColor)
	SLATE_END_ARGS()

	SCJSlateSpeechBubble();
	~SCJSlateSpeechBubble();

	void Construct(const FArguments& InArgs);

	// Update the bubble message
	void SetMessage(const FText& NewMessage);

	FSlateFontInfo m_ffontinfo = FCoreStyle::Get().GetFontStyle("EmbossedText");
	
	// The message text
	FText m_tMessage;

	// The bubble color
	FSlateColor m_fBackgroundColor;
};
