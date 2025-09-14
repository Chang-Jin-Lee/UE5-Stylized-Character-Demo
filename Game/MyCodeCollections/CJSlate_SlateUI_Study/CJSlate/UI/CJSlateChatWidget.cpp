// Fill out your copyright notice in the Description page of Project Settings.


#include "CJSlateChatWidget.h"

#include "CJSlate/CJSlateCharacter.h"
#include "CJSlate/CJSlatePlayerController.h"
#include "CJSlate/CJSlatePlayerState.h"
#include "Widgets/Layout/SWrapBox.h"

CJSlateChatWidget::CJSlateChatWidget()
{
}

CJSlateChatWidget::~CJSlateChatWidget()
{
}

#define LOCTEXT_NAMESPACE "CJSlateChatWidget"

void CJSlateChatWidget::Construct(const FArguments& InArgs) // 처음 생성됐을 때 실행 될 함수
{
    m_hudOwningHUDptr = InArgs._OwningHUD;
    
    fontinfo.Size = 24.0f; // 폰트 사이즈

    ChildSlot // 채팅 박스의 기본 구조를 생성
        .VAlign(VAlign_Bottom)
        .HAlign(HAlign_Left)
        .Padding(15) //코너에서 조금 띄우기
        [
            SNew(SVerticalBox) // outter container, 컨테이너의 밖
            + SVerticalBox::Slot()
            .AutoHeight()
            .MaxHeight(408.f)
            .VAlign(VAlign_Bottom)
            [
                SAssignNew(m_ListViewWidgetptr, SListView< TSharedPtr<FSChatMsgInfo>>) // 메시지 배열을 받아 HUD에 UI를 그리기
                .ListItemsSource(&m_rItems) // 이 listview에 대한 source가 들어있는 Items array를 전달
                .OnGenerateRow(this, &CJSlateChatWidget::OnGenerateRowForList) // 전달한 정보를 가지고 UI에 그리기를 시도
                .ScrollbarVisibility(EVisibility::Hidden) // 스크롤 바는 보이지 않게, 보이고 싶다면 주석처리.
            ]
            + SVerticalBox::Slot()
            .AutoHeight()
            .FillHeight(30.f)
            [
                SNew(SHorizontalBox)
                + SHorizontalBox::Slot()
                .AutoWidth()
                .MaxWidth(1000.f)
                [
                    SAssignNew(m_seditabletextChatInput, SEditableText) // 플레이어 input에 대한 widget
                    .OnTextCommitted(this, &CJSlateChatWidget::OnChatTextCommitted) // 텍스트 작성 후 엔터를 누르거나 보내기 버튼을 눌렀을 때 호출 될 함수
                    .OnTextChanged(this, &CJSlateChatWidget::OnChatTextChanged) // 메시지 텍스트를 작성할 때 호출 될 함수
                    .ClearKeyboardFocusOnCommit(true)
                    .Text(FText::FromString(TEXT("")))
                    .Font(fontinfo)
                    // .Font(FSlateFontInfo(fontinfo.FontMaterial, fontinfo.Size + 2)) // 추가 중인 폰트를 2 사이즈 만큼 크게 만들고 싶다면 주석 제거
                    .ColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 0.9f)) // 작성 중인 글씨 색상, R G B A
                    .HintText(FText::FromString(TEXT("Send a message to everyone."))) // 힌트 메시지 (필요 없다면 제거)
                ]
            ]
        ];
}

//** 여기에 Item.Get()->Type을 추가하여 귓속말 등 다른 로직을 추가 가능  */
TSharedRef<ITableRow> CJSlateChatWidget::OnGenerateRowForList(TSharedPtr<FSChatMsgInfo> Item, const TSharedRef<STableViewBase>& OwnerTable)
{
    if (!m_rItems.IsValidIndex(0) || !Item.IsValid() || !Item.Get()) // 에러가 있다면 채팅 메시지를 만들지 않음 
        return
            SNew(STableRow<TSharedPtr<FSChatMsgInfo>>, OwnerTable)
            [
                SNew(SBox)
            ];

    if (Item.Get()->m_iMessageType == 1) // Type 1은 클라에서 만들어질 채팅 메시지
    {
        return
            SNew(STableRow< TSharedPtr< FSChatMsgInfo > >, OwnerTable)
            [
                SNew(SWrapBox)
                .PreferredSize(600.0f)
                //.PreferredWidth(600.f)
                + SWrapBox::Slot()
                [
                    SNew(STextBlock) // 1. 시간을 보여주기
                    .Text(Item.Get()->m_tTimestamp)
                    .ColorAndOpacity(FLinearColor(0.25f, 0.25f, 0.25f, 1.f))
                    .Font(fontinfo)
                    .ShadowColorAndOpacity(FLinearColor::Black)
                    .ShadowOffset(FIntPoint(1, 1))
                ]
                + SWrapBox::Slot()
                [
                    SNew(STextBlock) // 2. username을 보여주기
                    .Text(Item.Get()->m_tUserName)
                    .ColorAndOpacity(FLinearColor::White)
                    .Font(fontinfo)
                    .ShadowColorAndOpacity(FLinearColor::Black)
                    .ShadowOffset(FIntPoint(1, 1))
                ]
                + SWrapBox::Slot()
                [
                    SNew(STextBlock) // 3. username과 채팅 메시지 사이에 " : " 보여주기
                    .Text(FText::FromString(" :  "))
                    .ColorAndOpacity(FLinearColor(0.5f, 0.5f, 0.5f, 1.f))
                    .Font(fontinfo)
                    .ShadowColorAndOpacity(FLinearColor::Black)
                    .ShadowOffset(FIntPoint(1, 1))
                ]
                + SWrapBox::Slot()
                [
                    SNew(STextBlock) //  4. 채팅 메시지 보여주기
                    .Text(Item.Get()->m_tText)
                    .ColorAndOpacity(FLinearColor(0.5f, 0.5f, 0.5f, 1.f))
                    .Font(fontinfo)
                    .ShadowColorAndOpacity(FLinearColor::Black)
                    .ShadowOffset(FIntPoint(1, 1))
                ]
            ];
    }
    // else if (Item.Get()->Type == 2) // Type 2는 서버 메시지 
    else
    {
        return
            SNew(STableRow< TSharedPtr< FSChatMsgInfo > >, OwnerTable)
            [
                SNew(SWrapBox)
                .PreferredSize(600.0f)
                //.PreferredWidth(600.f)
                + SWrapBox::Slot()
                [
                    SNew(STextBlock)    // 1. 시간 보여주기
                    .Text(Item.Get()->m_tTimestamp)
                    .ColorAndOpacity(FLinearColor(0.25f, 0.25f, 0.25f, 1.f))
                    .Font(fontinfo)
                    .ShadowColorAndOpacity(FLinearColor::Black)
                    .ShadowOffset(FIntPoint(1, 1))
                ]
                + SWrapBox::Slot()
                [
                    SNew(STextBlock)    // 2. 채팅 메시지 보여주기
                    .Text(Item.Get()->m_tText)
                    .ColorAndOpacity(FLinearColor(0.75f, 0.75f, 0.75f, 1.f))
                    .Font(fontinfo)
                    .ShadowColorAndOpacity(FLinearColor::Black)
                    .ShadowOffset(FIntPoint(1, 1))
                ]
                // 여기에 서버에서 보여 줄 메시지 내용을 추가
            ];
    }
}

void CJSlateChatWidget::OnChatTextChanged(const FText& InText) // 인풋 창에 키를 입력했을 때 매번 호출 될 함수
{
    FString SText = InText.ToString();
    if (SText.Len() > 120) // 인풋 창 최대 입력 글자 수는 120자로 제한 
    {
        SText = SText.Left(120);
        if (m_seditabletextChatInput.IsValid())
            m_seditabletextChatInput->SetText(FText::FromString(SText));
    }
}

void CJSlateChatWidget::OnChatTextCommitted(const FText& InText, ETextCommit::Type CommitMethod) // 작성한 채팅을 Sumitted 했을 때
{
    if (CommitMethod != ETextCommit::OnEnter) // 엔터키로 제출하지 않았다면 실행하지 않음
        return;

    if (m_hudOwningHUDptr->m_pcCurrentPlayerController)    // Player Controller의 마우스와 InputMode를 변경
    {
        m_hudOwningHUDptr->m_pcCurrentPlayerController->bShowMouseCursor = false;
        m_hudOwningHUDptr->m_pcCurrentPlayerController->SetInputMode(FInputModeGameOnly());
    }

    if (m_seditabletextChatInput.IsValid())
    {
        FText NFText = FText::TrimPrecedingAndTrailing(InText); // 텍스트의 양쪽 끝 공백을 제거
        if (!NFText.IsEmpty())
        {
            // rpc 함수를 쓰기 위해 PlayerState로 캐스팅하기
            if (ACJSlatePlayerState* ClientPS = Cast<ACJSlatePlayerState>(m_hudOwningHUDptr->m_pcCurrentPlayerController->PlayerState))
            {
                // 명령어에 따른 행동처리 로직
                // 해당 명령어는 rpc 함수를 실행시키지 않음
                // ex) sudo change color yellow
                FString Text = NFText.ToString();
                TArray<FString> args;
                Text.ParseIntoArray(args, TEXT(" "), true); // 공백을 기준으로 배열에 담기

                int32 NewType = 1;
                if (args.Num() > 0) // 배열의 개수가 0개 이상이라면
                {
                    if (args[0] == TEXT("sudo")) // 배열의 첫 번째 텍스트가 sudo 라면 명령어. 아니라면 일반 메시지
                    {
                        NewType = 9999; // NewType을 9999로 설정
                    }
                }
                
                FSChatMsgInfo newmessage; // replication을 위해 메시지 구조체 하나 만들기
                newmessage.Init(NewType, FText::FromString(ClientPS->GetPlayerName()), NFText); // replication을 위한 메시지 구조체 초기화
                if (newmessage.m_iMessageType > 0)    // 타입을 가지고 있다면
                    ClientPS->Server_UserChatCommited(ClientPS, newmessage); // 서버에게 해당 메시지 구조체를 전달하기. 이후 서버에서는 Multicast로 모든 클라에게 메시지 구조체 전달.
            }
        }
        m_seditabletextChatInput->SetText(FText()); // 로직이 끝났으니 인풋창을 비우기
    }

    FSlateApplication::Get().SetUserFocusToGameViewport(0, EFocusCause::SetDirectly); // 플레이어의 Focus를 다시 게임 뷰포트로 설정
}

void CJSlateChatWidget::AddMessage(const FSChatMsgInfo& newmessage) // 메시지를 채팅 창 아래에 추가 시키는 함수
{
    int32 index = m_rItems.Add(MakeShareable(new FSChatMsgInfo())); //chatbox array 에 새 메시지를 추가
    if (m_rItems[index].IsValid())
    {
        m_rItems[index]->Init(newmessage.m_iMessageType, newmessage.m_tUserName, newmessage.m_tText); // 전달 받은 메시지로 FSChatMsgInfo에 데이터를 생성

        int32 Year, Month, Day, DayOfWeek, Hour, Minute, Second, Millisecond; //  timestamp, decay timer를 설정하기 위한 변수
        FPlatformTime::SystemTime(Year, Month, DayOfWeek, Day, Hour, Minute, Second, Millisecond); // 윈도우 플랫폼에서의 시간
        m_rItems[index]->SetTime(FText::FromString(FString::Printf(TEXT("[ %02d:%02d:%02d ] "), Hour, Minute, Second)), FPlatformTime::Seconds()); // 시간을 메시지에 담아주기

        m_ListViewWidgetptr->RequestListRefresh(); // 새로운 채팅을 받았으니 다음 틱에서 채팅 박스 자체를 업데이트하기
        m_ListViewWidgetptr->ScrollToBottom(); // 새로운 채팅이 추가되었으니 채팅 박스를 맨 아래로 스크롤 내리기. 새 메시지가 맨 아래에 등장
    }
}

void CJSlateChatWidget::Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime) // 로직을 사용하기 위해 매 프레임 실행될 함수
{
    SCompoundWidget::Tick(AllottedGeometry, InCurrentTime, InDeltaTime);

    if (m_rItems.Num()) // 적어도 하나 이상의 채팅이 있음을 보장
    {
        if (!m_rItems[0]->m_bCreated) // 만약 생성된 시간이 없다면 영원히 지속되므로, 생성된 시간을 지금 시간으로 설정
            m_rItems[0]->m_bCreated = InCurrentTime;
        if (InCurrentTime - m_rItems[0]->m_bCreated > 20) // 20초 이상 지난 메시지인지 판단
        {
            m_rItems[0]->Destroy(); // 포인터와 변수를 초기값으로 초기화 
            m_rItems.RemoveAt(0); // 배열에서 제거하기
            m_rItems.Shrink(); // 배열이 사용하는 메모리를 가장 작게 최적화하기
        }
    }
}

#undef LOCTEXT_NAMESPACE