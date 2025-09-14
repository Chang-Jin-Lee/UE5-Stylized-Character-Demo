// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CJSlate/HUD/CJSlateHUD.h"

/**
 * 
 */
class CJSLATE_API CJSlateChatWidget : public SCompoundWidget
{
	// Slate Widget에서 배개변수로 받을 변수를 지정. Construct에 FArguments로 전달 됨.
	SLATE_BEGIN_ARGS(CJSlateChatWidget){}
	SLATE_ARGUMENT(TWeakObjectPtr<class ACJSlateHUD>, OwningHUD)
	SLATE_END_ARGS()
	
public:
	CJSlateChatWidget();
	~CJSlateChatWidget();
	
	TWeakObjectPtr<class ACJSlateHUD> m_hudOwningHUDptr; // 처음 생성될 때 Player Controller에 있는 HUD를 가져오기

	void Construct(const FArguments& InArgs);

	TSharedRef<ITableRow> OnGenerateRowForList(TSharedPtr<FSChatMsgInfo> Item, const TSharedRef<STableViewBase>& OwnerTable); // 채팅 상자에 표시될 각 채팅 요소에 대해 호출되는 함수
	TArray<TSharedPtr<FSChatMsgInfo>> m_rItems; // 플레이어의 채팅박스에 현재 들어있는 모든 아이템들 
	TSharedPtr< SListView< TSharedPtr<FSChatMsgInfo> > > m_ListViewWidgetptr; // 각 채팅 요소에 대한 실제 위젯

	// const FSlateFontInfo fontinfo = FSlateFontInfo(FPaths::EngineContentDir() / TEXT("UI/Fonts/Comfortaa-Regular.ttf"), 15); // 채팅 박스에 대한, 폰트 설정
	FSlateFontInfo fontinfo = FCoreStyle::Get().GetFontStyle("EmbossedText"); // 채팅 박스에 대한, 폰트 설정

	TSharedPtr< SVerticalBox > m_sverticalChatBox;
	TSharedPtr< SEditableText > m_seditabletextChatInput; // 플레이어 input에 대한 widget

	void OnChatTextChanged(const FText& InText);
	void OnChatTextCommitted(const FText& InText, ETextCommit::Type CommitMethod);

	void AddMessage(const FSChatMsgInfo& newmessage); // 채팅창에서 추가된 메시지를 맨 아래에 위치 시키기 
	void Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime); // 시간이 지난 메시지를 제거하는 Tick 함수
};
