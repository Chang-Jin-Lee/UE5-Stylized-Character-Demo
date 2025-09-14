// Fill out your copyright notice in the Description page of Project Settings.


#include "CJSlatePlayerState.h"

#include "CJSlateCharacter.h"
#include "CJSlatePlayerController.h"
#include "UI/CJSlateChatWidget.h"

ACJSlatePlayerState::ACJSlatePlayerState()
{
	
// 서버에서만 CommandTable을 가져와서 초기화 시켜주기
#if WITH_SERVER_CODE
	static ConstructorHelpers::FObjectFinder<UDataTable> CommandTableRef(TEXT("/Game/DataTable/CommandData.CommandData"));
	if (CommandTableRef.Succeeded())
	{
		m_dCommandTable = CommandTableRef.Object;
	}
#endif
}

void ACJSlatePlayerState::BeginPlay()
{
	Super::BeginPlay();
	
#if WITH_SERVER_CODE
	if (!m_dCommandTable) return; // 서버에서만 CommandTable을 로드하도록 제한

	TArray<FName> RowNames = m_dCommandTable->GetRowNames();
	for (const FName& RowName : RowNames)
	{
		FCommandData* CommandData = m_dCommandTable->FindRow<FCommandData>(RowName, TEXT(""));
		if (!CommandData) continue;

		FString Command = CommandData->m_sCommandName;
		FString FunctionName = CommandData->m_sFunctionName;

		m_mCommandMap.Add(Command, FunctionName);
	}
#endif
}

void ACJSlatePlayerState::Client_ReceiveCommandResponse_Implementation(const FString& Message)
{
	FSChatMsgInfo newmessage;
	newmessage.Init(9998, FText::FromString("From Server"), FText::FromString(Message));
	if ( ACJSlateHUD* TargetHud = Cast<ACJSlateHUD>(GetPlayerController()->GetHUD()))
		if (TargetHud && TargetHud->m_wChatUIWidgetptr.IsValid())
			TargetHud->m_wChatUIWidgetptr->AddMessage(newmessage); // TargetController에게 메시지 놓기
}

bool ACJSlatePlayerState::ExecuteCommandInternal(ACJSlatePlayerState* Executor, const FString& FunctionName,
                                                 const TArray<FString>& Params, FString& OutResult)
{
#if WITH_SERVER_CODE
	UE_LOG(LogTemp, Display, TEXT("ExecuteCommandInternal"));
	UE_LOG(LogTemp, Display, TEXT("FunctionName : %s"), *FunctionName);
	UFunction* Function = FindFunction(FName(*FunctionName));
	if (!Function)
	{
		OutResult = TEXT("Invalid command.");
		return false;
	}

	struct FCommandParams
	{
		ACJSlatePlayerState* Executor;
		TArray<FString> Params;
		FString Result;
		bool bSuccess;
	};

	FCommandParams CommandParams;
	CommandParams.Executor = Executor;
	CommandParams.Params = Params;
	CommandParams.bSuccess = false;

	ProcessEvent(Function, &CommandParams);

	OutResult = CommandParams.Result;
	return CommandParams.bSuccess;
#else
	return false;
#endif
}

void ACJSlatePlayerState::ExecuteChangeCharacterColor(ACJSlatePlayerState* Executor, const TArray<FString>& Params,
	FString& OutResult)
{
	if (!Executor) 
	{
		OutResult = TEXT("Error: No executor.");
		return;
	}
	if (Params.Num() < 1)
	{
		OutResult = TEXT("Usage: sudo change_character_color <ColorName>");
		return;
	}

	if (ACharacter* ch = Cast<ACharacter>(Executor->GetPawn()))
	{
		if (FLinearColor* color = m_mColorFromStringMap.Find(Params[0]))
		{
			Executor->Server_ChangeChatracterColor(ch, *color);
		}
		else
		{
			OutResult = TEXT("Color Not Exist");
			return;
		}
	}
	else
	{
		OutResult = FString::Printf(TEXT("Error: Character not found in ExecuteChangeCharacterColor"));
		return;
	}

	
	OutResult = FString::Printf(TEXT("Player Color has been Changed"));
}

bool ACJSlatePlayerState::Server_ChangeChatracterColor_Validate(ACharacter* Character, const FLinearColor Color)
{
	return true;
}

void ACJSlatePlayerState::Client_ChangeChatracterColor_Implementation(ACharacter* Character, const FLinearColor Color)
{
	if (ACJSlateCharacter* ch = Cast<ACJSlateCharacter>(Character))
	{
		ch->ChangeMaterialColor(Color);
	}
}

bool ACJSlatePlayerState::Client_ChangeChatracterColor_Validate(ACharacter* Character, const FLinearColor Color)
{
	return true;
}

void ACJSlatePlayerState::Server_ChangeChatracterColor_Implementation(ACharacter* Character, const FLinearColor Color)
{
	Client_ChangeChatracterColor(Character, Color);
}

bool ACJSlatePlayerState::Server_UserChatCommited_Validate(ACJSlatePlayerState* SenderState, const FSChatMsgInfo& newmessage)
{
	// SenderState가 nullptr이면 실행하지 않음.
	return SenderState != nullptr;
}
void ACJSlatePlayerState::Server_UserChatCommited_Implementation(ACJSlatePlayerState* SenderState, const FSChatMsgInfo& newmessage)
{
#if WITH_SERVER_CODE
	// 이 부분에 타입에 따른 메시지 로직 추가
	if (newmessage.m_iMessageType == 9999) // 서버에서 명령어를 요청했다면. 만약 외부 서버와 연결이 필요하다면 이 부분에 추가
	{
		FString Text = newmessage.m_tText.ToString();
		TArray<FString> args;
		Text.ParseIntoArray(args, TEXT(" "), true); // 공백을 기준으로 배열에 담기
		// if (args.Num() > 0) // 배열의 개수가 0개 이상이라면
		// {
		// 	if (args[0] == TEXT("sudo")) // 배열의 첫 번째 텍스트가 sudo 라면 명령어. 아니라면 일반 메시지
		// 	{
		// 		if (args.Num() >= 4)    // 4 단어 명령어
		// 		{
		// 			if (args[1].ToLower() == TEXT("change") && args[2].ToLower() == TEXT("color")) // 캐릭터의 색을 변경하는 로직
		// 			{
		// 				if (ACharacter* ch = Cast<ACharacter>(SenderState->GetPawn()))
		// 				{
		// 					if (FLinearColor* color = ColorFromStringMap.Find(args[3]))
		// 						SenderState->Server_ChangeChatracterColor(ch, *color);
		// 				}
		// 			}
		// 		}
		// 		else if (args.Num() >= 3)   // 3 단어 명령어
		// 		{
		// 			if (args[1].ToLower() == TEXT("servertravel"))  // 서버를 다른 레벨로 servertravel 하기
		// 			{
		// 				FString _level = args[2];
		// 				if (ACJSlatePlayerController* chp = Cast<ACJSlatePlayerController>(SenderState->GetPlayerController()))
		// 				{
		// 					chp->Server_ServerTravel(_level);
		// 				}
		// 			}
		// 		}
		// 		else if (args.Num() >= 2)   // 2 단어 명령어
		// 		{
		// 			if (args[1].ToLower() == TEXT("restart"))   // 서버의 레벨을 다시 servertravel 하기
		// 			{
		// 				if (ACJSlatePlayerController* chp = Cast<ACJSlatePlayerController>(SenderState->GetPlayerController()))
		// 				{
		// 					chp->Server_RestartLevel();
		// 				}
		// 			}
		// 		}
		// 	}
		// }

		if (args.Num() > 1)
		{
			FCommandData* CommandData = m_dCommandTable->FindRow<FCommandData>(*args[1], TEXT(""));
			if (!CommandData)
			{
				Client_ReceiveCommandResponse(TEXT("Unknown command."));
				return;
			}

			// 서버 전용 명령어인지 확인
			if (CommandData->m_bServerOnly && !HasAuthority())
			{
				Client_ReceiveCommandResponse(TEXT("Permission denied."));
				return;
			}

			TArray<FString> Params;
			for (int32 i = 2; i < args.Num(); i++) Params.Add(args[i]);
			// 명령어 실행
			FString CommandResult;
			bool bSuccess = ExecuteCommandInternal(SenderState, CommandData->m_sFunctionName, Params, CommandResult);
			// 실행 결과 클라이언트에게 전송
			Client_ReceiveCommandResponse(CommandResult);
		}
	}
	else // 명령어가 아니라면 모든 클라에게 메시지 전달. 
	{
		Multicast_UserChatCommited(newmessage);
	}
#endif
}

bool ACJSlatePlayerState::Multicast_UserChatCommited_Validate(const FSChatMsgInfo& newmessage)
{
	return true;
}
void ACJSlatePlayerState::Multicast_UserChatCommited_Implementation(const FSChatMsgInfo& newmessage)
{
	APlayerController* TargetController = nullptr;
	APlayerController* SourceController = GetPlayerController();

	// 나의 머리 위에 말풍선을 다른 클라들에게 보여주기
	if (SourceController)
		if (ACJSlateCharacter* SourceCharacter = Cast<ACJSlateCharacter>(SourceController->GetCharacter()))
			SourceCharacter->Multicast_UpdateMessage(newmessage);

	// 클라이언트에 있는 모든 컨트롤러를 찾기
	// 모든 클라들에게 메시지 전달하기
	for (FConstPlayerControllerIterator Iterator = GetWorld()->GetPlayerControllerIterator(); Iterator; ++Iterator)
	{
		TargetController = Cast<APlayerController>(*Iterator);

		if (TargetController)
			if ( ACJSlateHUD* TargetHud = Cast<ACJSlateHUD>(TargetController->GetHUD()))
				if (TargetHud && TargetHud->m_wChatUIWidgetptr.IsValid())
					TargetHud->m_wChatUIWidgetptr->AddMessage(newmessage); // TargetController에게 메시지 놓기
	}
}
