// Fill out your copyright notice in the Description page of Project Settings.


#include "SCJSlateSpeechBubble.h"
#include "SlateOptMacros.h"
#include "StaticMeshAttributes.h"
#include "CJSlate/CJSlateCharacter.h"

#define LOCTEXT_NAMESPACE "SCJSlateSpeechBubble"

SCJSlateSpeechBubble::SCJSlateSpeechBubble()
{
}

SCJSlateSpeechBubble::~SCJSlateSpeechBubble()
{
}

void SCJSlateSpeechBubble::Construct(const FArguments& InArgs)
{
	m_tMessage = InArgs._Message;
	m_fBackgroundColor = InArgs._BackgroundColor;
	
	m_ffontinfo.Size = 16.0f;

	ChildSlot
	[
		SNew(SBorder)
		.BorderBackgroundColor(m_fBackgroundColor)
		.Padding(FMargin(10.0f))
		[
			SNew(STextBlock)
			.Text(m_tMessage)
			.Font(m_ffontinfo)
			.ColorAndOpacity(FLinearColor::White)
			.Justification(ETextJustify::Center)
			.ShadowColorAndOpacity(FLinearColor::Black)
			.ShadowOffset(FVector2D(1.0f, 1.0f))
		]
	];
}

// 메시지를 쳤을 때 캐릭터 머리 위에 생기는 말풍선
void SCJSlateSpeechBubble::SetMessage(const FText& NewMessage)
{
	m_tMessage = NewMessage;
	
	// 전달 받은 메시지로 말풍선 UI를 캐릭터 머리 위에 띄우기
	if (ChildSlot.GetWidget()->GetTypeAsString() == TEXT("SBorder"))
	{
		auto BorderWidget = StaticCastSharedRef<SBorder>(ChildSlot.GetWidget());
		BorderWidget->SetContent(
			SNew(STextBlock)
			.Text(m_tMessage)
			.Font(m_ffontinfo)
			.ColorAndOpacity(FLinearColor::White)
			.ShadowColorAndOpacity(FLinearColor::Black)
			.ShadowOffset(FVector2D(1.0f, 1.0f))
		);
	}
}

#undef LOCTEXT_NAMESPACE