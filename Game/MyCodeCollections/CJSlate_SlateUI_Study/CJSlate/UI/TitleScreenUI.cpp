// Fill out your copyright notice in the Description page of Project Settings.


#include "TitleScreenUI.h"

#include "Components/Border.h"
#include "Components/Button.h"
#include "Components/CircularThrobber.h"
#include "Components/EditableText.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"

void UTitleScreenUI::NativeConstruct()
{
	Super::NativeConstruct();
	ConnectServerButton->OnClicked.AddDynamic(this, &UTitleScreenUI::ConnectServer);
	QuitButton->OnClicked.AddDynamic(this, &UTitleScreenUI::Quit);
	EditTextIpAdress->OnTextChanged.AddDynamic(this, &UTitleScreenUI::InputText);
	EditTextIpAdress->OnTextCommitted.AddDynamic(this, &UTitleScreenUI::InputTextCommited);
	PopDown();
}

// 여기서 서버에게 어떤 포트에 연결이 가능한지를 물어봐야함.
void UTitleScreenUI::ConnectServer()
{
	APlayerController* PC = UGameplayStatics::GetPlayerController(this,0);
	if (PC)
	{
		FString t = IpAdress.ToString();
		PC->ClientTravel(t, TRAVEL_Absolute);
		PopUp(FText::FromString("Connecting... Please Wait"));
		PlayAnimation_GuideFadeIn();
		// GetWorld()->GetTimerManager().SetTimer(GuideWidgetTimerHandle, this,  &UTitleScreenUI::PlayAnimation_GuideFadeIn, 15.0f);
	}
}

void UTitleScreenUI::Quit()
{
	if (APlayerController* PC = UGameplayStatics::GetPlayerController(this,0))
	{
		PC->ConsoleCommand("quit");
		//PC->ExitMenu();
	}
}

void UTitleScreenUI::InputText(const FText& _value)
{
	IpAdress = _value;
}

void UTitleScreenUI::InputTextCommited(const FText& _value, ETextCommit::Type _commited)
{
	if (_commited == ETextCommit::Type::OnEnter)
	{
		IpAdress = _value;
		ConnectServer();
	}
}

void UTitleScreenUI::PlayAnimation_GuideFadeIn()
{
	PlayAnimation(GuideFadein);
	
	GetWorld()->GetTimerManager().ClearTimer(GuideWidgetTimerHandle);
	GetWorld()->GetTimerManager().SetTimer(GuideWidgetTimerHandle, this,  &UTitleScreenUI::PlayAnimation_GuideFadeOut, TimeOutValue + 3);
	GetWorld()->GetTimerManager().ClearTimer(GuideWidgetTimerHandleFadeOut);
	GetWorld()->GetTimerManager().SetTimer(GuideWidgetTimerHandleFadeOut, this,  &UTitleScreenUI::TimeOut, TimeOutValue);
}

void UTitleScreenUI::PlayAnimation_GuideFadeOut()
{
	PlayAnimation(GuideFadeOut);
}

void UTitleScreenUI::TimeOut()
{
	PopUp(FText::FromString("TimeOut! Please Try Other IP"));
	PlayAnimation(GuideFadein);
}
