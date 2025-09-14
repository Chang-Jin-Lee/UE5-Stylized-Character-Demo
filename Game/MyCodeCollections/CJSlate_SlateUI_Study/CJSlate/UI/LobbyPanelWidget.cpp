// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyPanelWidget.h"

#include "OnlineSessionSettings.h"
#include "OnlineSubsystem.h"
#include "OnlineSubsystemUtils.h"
#include "CJSlate/CJSlateGameMode.h"
#include "CJSlate/CJSlatePlayerController.h"
#include "CJSlate/GameInstance/CJSlateGameInstance.h"
#include "CJSlate/HUD/LobbyHUD.h"
#include "Components/Button.h"
#include "Components/CheckBox.h"
#include "Components/CircularThrobber.h"
#include "Components/EditableText.h"
#include "Components/EditableTextBox.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"

void ULobbyPanelWidget::NativeConstruct()
{
	Super::NativeConstruct();
	ReturnButton->OnClicked.AddDynamic(this, &ULobbyPanelWidget::Return);
	HostButton->OnClicked.AddDynamic(this, &ULobbyPanelWidget::CreateHost);
	JoinButton->OnClicked.AddDynamic(this, &ULobbyPanelWidget::JoinGame);
	QuitButton->OnClicked.AddDynamic(this, &ULobbyPanelWidget::QuitGame);
	ClientTravelButton->OnClicked.AddDynamic(this, &ULobbyPanelWidget::ClientTravel);
	ServerTravelButton->OnClicked.AddDynamic(this, &ULobbyPanelWidget::ServerTravel);
	UseLanCheckBox->OnCheckStateChanged.AddDynamic(this, &ULobbyPanelWidget::UseLANToggle);
	EditTextBlockJoinIPAdress->OnTextChanged.AddDynamic(this, &ULobbyPanelWidget::IPAdressChanged);
	EditTextBlockSesionName->OnTextChanged.AddDynamic(this, &ULobbyPanelWidget::SessionNameChanged);
	PopDown();
	bUseLan = true;
	UpdatePlayerCounts();
}

void ULobbyPanelWidget::Return()
{
	if (UCJSlateGameInstance* gi = Cast<UCJSlateGameInstance>(GetGameInstance()))
	{
		gi->EndSession(gi->m_frequestedSessionName);
		// gi->DestroySession(FName(*SessionName.ToString()));
	}
	
	APlayerController* PC = UGameplayStatics::GetPlayerController(this,0);
	if (PC)
	{
		PC->ClientTravel("Title", TRAVEL_Absolute);
	}
}

void ULobbyPanelWidget::CreateHost()
{
	IOnlineSubsystem* OnlineSubsystem = Online::GetSubsystem(GetWorld());
	if (OnlineSubsystem)
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(
				-1,
				15.f,
				FColor::Blue,
				FString::Printf(TEXT("Found Subsystem %s"), *OnlineSubsystem->GetSubsystemName().ToString())
			);
		}
	}

	if (SessionName.IsEmpty())
	{
		PopUp(FText::FromString("Please Fill in SessionName"));
		return;
	}

	ACJSlatePlayerController* pc = Cast<ACJSlatePlayerController>(GetOwningPlayer());
	if (pc)
	{
		UE_LOG(LogTemp, Warning, TEXT("[Client] PlayerController에게 세션 생성 요청"));
		// pc->Server_RequestCreateSession(5,SessionName.ToString(),54321,bUseLan);
		pc->Server_ServerTravel(SessionName.ToString());
	}
	// if ( ALobbyHUD* hud = Cast<ALobbyHUD>(UGameplayStatics::GetPlayerController(this, 0)->GetHUD()))
	// {
	// 	hud->Server_RequestCreateSession(5, 54321, bUseLan);
	// }
	
	// if ( ACJSlateGameMode* gm = Cast<ACJSlateGameMode>(UGameplayStatics::GetGameMode((this))))
	// {
	// 	gm->Server_RequestCreateSession(5,54321,bUseLan);
	// }
	
	// if (UCJSlateGameInstance* gi = Cast<UCJSlateGameInstance>(GetGameInstance()))
	// {
	// 	gi->CreateSession(5, 54321, bUseLan);
	// }
}

void ULobbyPanelWidget::JoinGame()
{
	if (bUseLan)
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Use LAN!"));
	else
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Not Use LAN!"));

	// 클라쪽에서 검색하기
	if (UCJSlateGameInstance* gi = Cast<UCJSlateGameInstance>(GetGameInstance()))
	{
		PopUp(FText::FromString("Connecting..."));
		gi->FindSessions(100, FName(*SessionName.ToString()), bUseLan);
		// FOnlineSessionSearchResult* SessionResult = nullptr;
		// gi->JoinGameSession(*SessionResult);
		// APlayerController* playerController = GetWorld()->GetFirstPlayerController();
		// playerController->ClientTravel("IpAdress:7777", TRAVEL_Absolute);
	}

	// APlayerController* PC = GetOwningPlayer();
	// if (PC)
	// {
	// 	ACJSlatePlayerController* cjpc = Cast<ACJSlatePlayerController>(PC);
	// 	if (cjpc)
	// 	{
	// 		UE_LOG(LogTemp, Warning, TEXT("[Client] PlayerController에게 세션 Join 요청"));
	// 		cjpc->Server_RequestJoinSession(5,SessionName.ToString(),54321,bUseLan);
	// 	}
	// }
}

void ULobbyPanelWidget::ClientTravel()
{
	APlayerController* PC = UGameplayStatics::GetPlayerController(this,0);
	if (PC)
	{
		FString t = IpAdress.ToString();
		PC->ClientTravel(t, TRAVEL_Absolute);
	}
}

void ULobbyPanelWidget::ServerTravel()
{
	if (IpAdress.IsEmpty() == false)
	{
		ACJSlatePlayerController* cp = Cast<ACJSlatePlayerController>(GetOwningPlayer());
		if (cp)
		{
			cp->Server_ServerTravel(IpAdress.ToString());
		}
	}
}

void ULobbyPanelWidget::QuitGame()
{
	if (APlayerController* PC = UGameplayStatics::GetPlayerController(this,0))
	{
		PC->ConsoleCommand("quit");
		//PC->ExitMenu();
	}
}

void ULobbyPanelWidget::UseLANToggle(bool _bIsChecked)
{
	if (_bIsChecked == true)
		bUseLan = true;
	else
		bUseLan = false;
}

void ULobbyPanelWidget::IPAdressChanged(const FText& _value)
{
	if (_value.IsEmpty() == false)
		IpAdress = _value;
}

void ULobbyPanelWidget::SessionNameChanged(const FText& _value)
{
	if (_value.IsEmpty() == false)
		SessionName = _value;
}

void ULobbyPanelWidget::UpdatePlayerCounts()
{
	ACJSlatePlayerController* cp = Cast<ACJSlatePlayerController>(GetOwningPlayer());
	if (cp)
	{
		TextBlock_ConnectedPeople->SetText(FText::FromString(FString::FromInt(cp->CurrentPlayerCount)));
	}
}

void ULobbyPanelWidget::PlayAnimation_GuideFadeOut()
{
}

void ULobbyPanelWidget::PlayAnimation_GuideFadeIn()
{
}

void ULobbyPanelWidget::TimeOut()
{
}
