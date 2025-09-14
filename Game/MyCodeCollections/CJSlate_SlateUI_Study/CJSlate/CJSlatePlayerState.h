// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CJSlate/HUD/CJSlateHUD.h"
#include "GameFramework/PlayerState.h"
#include "CJSlatePlayerState.generated.h"

// 명령어 - 함수 매핑을 위한 구조체
USTRUCT()
struct FCommandData : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CommandData")
	FString m_sCommandName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CommandData")
	FString m_sFunctionName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CommandData")
	bool m_bServerOnly = true;
};

//  명령어의 결과. 에러메시지, 성공메시지를 전달할때 쓰임
struct FCommandResult
{
	bool m_bSuccess;
	FString m_sErrorMessage;

	FCommandResult(bool bInSuccess, FString InErrorMessage = TEXT(""))
		: m_bSuccess(bInSuccess), m_sErrorMessage(InErrorMessage) {}
};

/**
 * 
 */
UCLASS()
class CJSLATE_API ACJSlatePlayerState : public APlayerState
{
	GENERATED_BODY()
public:
	ACJSlatePlayerState();

	virtual void BeginPlay() override;

	// 채팅 시스템
	// Server_UserChatCommited 함수에 외부 서버와의 통신, DB에 기록 저장 등의 로직을 추가하면 됨
	UFUNCTION(Server, Reliable, WithValidation) //클라에서 서버에게 message를 전달하는 첫 번째 함수
    virtual void Server_UserChatCommited(ACJSlatePlayerState* SenderState, const FSChatMsgInfo& newmessage); // 서버에서 실행될 함수 
    UFUNCTION(NetMulticast, Reliable, WithValidation) // 서버에서 모든 클라에게 멀티캐스트로 함수를 실행
    virtual void Multicast_UserChatCommited(const FSChatMsgInfo& newmessage); // 모든 클라이언트에서 실행 될 함수

	// 캐릭터의 색을 변경하는 명령어 처리 부분
	UFUNCTION(Server, Reliable, WithValidation)
	virtual void Server_ChangeChatracterColor(ACharacter* Character, const FLinearColor Color);
	UFUNCTION(NetMulticast, Reliable, WithValidation)
	virtual void Client_ChangeChatracterColor(ACharacter* Character, const FLinearColor Color);

	UFUNCTION(Client, Reliable)
	void Client_ReceiveCommandResponse(const FString& Message);

private:	// 서버에서 쓰일 명령어 관련 변수와 함수 private로 선언
	UPROPERTY()
	UDataTable* m_dCommandTable = nullptr;	// 명령어 함수 매핑을 위한 데이터 테이블
	
	TMap<FString, FString> m_mCommandMap;
	// TMap<FString, TFunction<FCommandResult(APlayerController*, const FString&)>> CommandMap;
	
	bool ExecuteCommandInternal(ACJSlatePlayerState* Executor, const FString& FunctionName, const TArray<FString>& Params, FString& OutResult);

	// 명령어에 해당하는 함수
	// change_character_color
	UFUNCTION(Exec)
	void ExecuteChangeCharacterColor(ACJSlatePlayerState* Executor, const TArray<FString>& Params, FString& OutResult);
	
	TMap<FString, FLinearColor> m_mColorFromStringMap = {
		{TEXT("White"), FLinearColor::White},
		{TEXT("Gray"), FLinearColor::Gray},
		{TEXT("Black"), FLinearColor::Black},
		{TEXT("Transparent"), FLinearColor::Transparent},
		{TEXT("Red"), FLinearColor::Red},
		{TEXT("Green"), FLinearColor::Green},
		{TEXT("Blue"), FLinearColor::Blue},
		{TEXT("Yellow"), FLinearColor::Yellow},

		// 추가 색상
		{TEXT("Orange"), FLinearColor(1.0f, 0.5f, 0.0f, 1.0f)},
		{TEXT("Cyan"), FLinearColor(0.0f, 1.0f, 1.0f, 1.0f)},
		{TEXT("Magenta"), FLinearColor(1.0f, 0.0f, 1.0f, 1.0f)},
		{TEXT("Purple"), FLinearColor(0.5f, 0.0f, 0.5f, 1.0f)},
		{TEXT("Pink"), FLinearColor(1.0f, 0.75f, 0.8f, 1.0f)},
		{TEXT("Lime"), FLinearColor(0.75f, 1.0f, 0.0f, 1.0f)},
		{TEXT("Teal"), FLinearColor(0.0f, 0.5f, 0.5f, 1.0f)},
		{TEXT("Brown"), FLinearColor(0.6f, 0.3f, 0.0f, 1.0f)},
		{TEXT("Gold"), FLinearColor(1.0f, 0.84f, 0.0f, 1.0f)},
		{TEXT("Silver"), FLinearColor(0.75f, 0.75f, 0.75f, 1.0f)},
		{TEXT("Indigo"), FLinearColor(0.29f, 0.0f, 0.51f, 1.0f)},
		{TEXT("Maroon"), FLinearColor(0.5f, 0.0f, 0.0f, 1.0f)},
		{TEXT("Navy"), FLinearColor(0.0f, 0.0f, 0.5f, 1.0f)},
		{TEXT("Olive"), FLinearColor(0.5f, 0.5f, 0.0f, 1.0f)},
		{TEXT("Turquoise"), FLinearColor(0.25f, 0.88f, 0.82f, 1.0f)},
		{TEXT("Salmon"), FLinearColor(0.98f, 0.5f, 0.45f, 1.0f)}
	};
};


