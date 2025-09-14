// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "CJSlateHUD.generated.h"
class UInventoryWidget;
/**
 * 
 */
USTRUCT()
struct FSChatMsgInfo // 메시지 데이터 구조체. 
{
	GENERATED_BODY()

	// 9999 : 서버에게 보낼 명령어 (sudo로 시작하는 명령어)
	// 9998 : 서버로부터 받은 응답 (sudo로 시작하는 명령어에 대한 실행 여부)
	// 1 : 일반 채팅
	UPROPERTY() // 채팅 메시지의 타입을 나타냄. 전체 채팅인지, 귓속말인지, 팀원 채팅인지 등.
	int32 m_iMessageType;
	
	UPROPERTY()
	FText m_tUserName;

	UPROPERTY()
	FText m_tText;

	FText m_tTimestamp; // Dont replicate time because we can set it locally once we receive the struct

	double m_bCreated;

	void Init(int32 NewType, FText NewUsername, FText NewText) // Assign only the vars we wish to replicate
	{
		m_iMessageType = NewType;
		m_tUserName = NewUsername;
		m_tText = NewText;
	}
	void SetTime(FText NewTimestamp, double NewCreated)
	{
		m_tTimestamp = NewTimestamp;
		m_bCreated = NewCreated;
	}
	void Destroy()
	{
		m_iMessageType = NULL;
		m_tUserName.GetEmpty();
		m_tText.GetEmpty();
		m_tTimestamp.GetEmpty();
		m_bCreated = NULL;
	}
};


UCLASS()
class CJSLATE_API ACJSlateHUD : public AHUD
{
	GENERATED_BODY()
	
public:
	// UI 관련
	TSharedPtr<class SCJSlateOptionMenuWidget> m_cMainMenuWidgetPtr;
	TSharedPtr<class SWidget> m_wMainMenuWidgetContainer;
	
	void ShowMenu();
	void ExitMenu();

	virtual void BeginPlay() override;

	////////////////////////////////////// chatting ///////////////////////////////////////////////
	TSharedPtr<class CJSlateChatWidget> m_wChatUIWidgetptr; // Reference to the main chat widget
	APlayerController* m_pcCurrentPlayerController;
	
	// HUD 창 조준선, 안내 메시지 등
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "UI")
	TSubclassOf<class UInGamePanelWidget> m_wHUDPanelWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "UI")
	TObjectPtr<class UInGamePanelWidget> m_wHUDPanelWidget;
	
	void OpenInventory();
	void DestoryInventory();

	UFUNCTION(BlueprintCallable, Category = "User")
	void AddMessageBP(const int32 Type, const FString& Username, const FString& Text, const bool Replicate); // A Blueprint function you can use to place messages in the chat box during runtime

protected:
	virtual void PostInitializeComponents() override; // 모든 채팅 관련 UI는 여기서 생성 
	virtual void DrawHUD() override; // 생성된 UI를 접근할 때 호출될 함수

	///////////////////////////////////////////////////////////////////////////////////////////////
};
