// Fill out your copyright notice in the Description page of Project Settings.


#include "CJSlateHUD.h"

#include "CJSlate/CJSlateCharacter.h"
#include "CJSlate/CJSlatePlayerState.h"
#include "Blueprint/UserWidget.h"
#include "CJSlate/UI/InGamePanelWidget.h"
#include "CJSlate/UI/CJSlateChatWidget.h"
#include "CJSlate/UI/InGamePanelWidget.h"
#include "CJSlate/UI/SCJSlateOptionMenuWidget.h"
#include "CJSlate/UI/Inventory/InventoryHoverWidget.h"
#include "CJSlate/UI/Inventory/InventoryItemClickOptionWidget.h"
#include "CJSlate/UI/Inventory/InventoryWidget.h"
#include "CJSlate/UI/Inventory/InventoryWidgetSlot.h"
#include "Widgets/SWeakWidget.h"

void ACJSlateHUD::BeginPlay()
{
	Super::BeginPlay();

	if (UInGamePanelWidget* widget = CreateWidget<UInGamePanelWidget>(GetWorld(), m_wHUDPanelWidgetClass))
	{
		m_wHUDPanelWidget = widget;
		m_wHUDPanelWidget->AddToViewport();
		// HUDPannelWidget->SetVisibility(ESlateVisibility::Hidden);
	}
}

void ACJSlateHUD::ShowMenu()
{
	if (APlayerController* PC = GetOwningPlayerController())
	{
		ACJSlateCharacter* ch = Cast<ACJSlateCharacter>(PC->GetCharacter());
		if (ch)
		{
			ch->bShowMainMenu = true;
		}
		if (PC->IsLocalController())
		{
			if (GEngine && GEngine->GameViewport)
			{
				//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::White, "Created MainMenuWidgetPtr");
				//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::White, FString::Printf(TEXT("%s Gengnie"), *GEngine->GetName()));
				// GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, FString::Printf(TEXT("%llu Gengnie ID"), MainMenuWidgetContainer->GetId()));
				m_cMainMenuWidgetPtr = SNew(SCJSlateOptionMenuWidget).OwningHUD(this);
				GEngine->GameViewport->AddViewportWidgetContent(SAssignNew(m_wMainMenuWidgetContainer, SWeakWidget).PossiblyNullContent(m_cMainMenuWidgetPtr.ToSharedRef()));
			}
			PC->bShowMouseCursor = true;
			PC->SetInputMode(FInputModeUIOnly());
		}
	}
}

void ACJSlateHUD::ExitMenu()
{
	if (APlayerController* PC = GetOwningPlayerController())
	{
		ACJSlateCharacter* ch = Cast<ACJSlateCharacter>(PC->GetCharacter());
		if (ch)
		{
			ch->bShowMainMenu = false;
		}
		if (PC->IsLocalController())
		{
			
			if (GEngine && GEngine->GameViewport && m_wMainMenuWidgetContainer.IsValid())
			{
				GEngine->GameViewport->RemoveViewportWidgetContent(m_wMainMenuWidgetContainer.ToSharedRef());
				//MainMenuWidgetContainer.Reset();
				// 강제로 Slate UI 갱신
				//FSlateApplication::Get().ForceRedrawWindow(GEngine->GameViewport->GetWindow().ToSharedRef());
			}
			PC->bShowMouseCursor = false;
			PC->SetInputMode(FInputModeGameOnly());
		}
	}
}


void ACJSlateHUD::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	if (GEngine && GEngine->GameViewport) // GEngine과 GameViewport에 대한 NULL 체크
	{
		SAssignNew(m_wChatUIWidgetptr, CJSlateChatWidget).OwningHUD(this); // 위젯을 생성하고 매개변수를 전달하기
		GEngine->GameViewport->AddViewportWidgetContent(SNew(SWeakWidget).PossiblyNullContent(m_wChatUIWidgetptr.ToSharedRef())); // 위젯을 ViewPort에 띄우기, zorder는 0으로
	}
}

void ACJSlateHUD::DrawHUD()
{
	Super::DrawHUD();

	if (!m_pcCurrentPlayerController)
	{
		m_pcCurrentPlayerController = GetOwningPlayerController();
		AddMessageBP(2, TEXT(""), TEXT("Welcome. Press Enter to chat."), false); // random Welcome message shown to the local player. To be deleted. note type 2 is system message and username is blank
		return;
	}

	if (m_pcCurrentPlayerController->WasInputKeyJustPressed(EKeys::Enter))
		if (m_wChatUIWidgetptr.IsValid() && m_wChatUIWidgetptr->m_seditabletextChatInput.IsValid())
			FSlateApplication::Get().SetKeyboardFocus(m_wChatUIWidgetptr->m_seditabletextChatInput); // 엔터키를 눌렀을 때 인풋 창에 포커스를 옮기기
}

void ACJSlateHUD::OpenInventory()
{
	if (m_wHUDPanelWidget->InventoryWidget)
	{
		m_wHUDPanelWidget->InventoryWidget->LoadData();
		m_wHUDPanelWidget->InventoryWidget->SetVisibility(ESlateVisibility::Visible);
	}
}

void ACJSlateHUD::DestoryInventory()
{
	if (m_wHUDPanelWidget->InventoryWidget)
	{
		m_wHUDPanelWidget->InventoryWidget->LoadData();
		m_wHUDPanelWidget->InventoryWidget->SetVisibility(ESlateVisibility::Hidden);
		for (auto slot : m_wHUDPanelWidget->InventoryWidget->InventoryWidgetSlots)
		{
			if (slot->UInventoryHoverWidgetptr)
			{
				slot->UInventoryHoverWidgetptr->RemoveFromParent();
			}
			if (slot->UInventoryItemClickOptionWidgetptr)
			{
				slot->UInventoryItemClickOptionWidgetptr->RemoveFromParent();
			}
		}
	}
}

void ACJSlateHUD::AddMessageBP(const int32 Type, const FString& Username, const FString& Text, const bool Replicate)
{
	if (!m_pcCurrentPlayerController || !m_wChatUIWidgetptr.IsValid())
		return;

	FSChatMsgInfo newmessage;
	newmessage.Init(Type, FText::FromString(Username), FText::FromString(Text)); // 메시지 구조체를 전달받은 값으로 초기화하기
	if (newmessage.m_iMessageType > 0)
	{
		if (Replicate)
		{
			ACJSlatePlayerState* ps = Cast<ACJSlatePlayerState>(m_pcCurrentPlayerController->PlayerState);
			if (ps)
				ps->Server_UserChatCommited(ps, newmessage); // 복제된 후 표시될 수 있도록 전체 채팅 메시지를 PlayerState로 보내기.
		}
		else
			m_wChatUIWidgetptr->AddMessage(newmessage); // 클라이언트에게만 로컬 메시지를 보냄. 다른 클라는 이를 받지 않음.
	}
}
