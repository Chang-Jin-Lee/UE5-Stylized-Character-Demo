// Fill out your copyright notice in the Description page of Project Settings.


#include "Lena_IntroSelectGameWidget.h"

#include "Animation/UMGSequencePlayer.h"
#include "Kismet/GameplayStatics.h"

bool ULena_IntroSelectGameWidget::Initialize()
{
	bool bSuccess = Super::Initialize();
	if(bSuccess == false) return false;
	
	if(Button_Room01)
	{
		Button_Room01->OnClicked.AddDynamic(this, &ULena_IntroSelectGameWidget::OnRoom01Animation);
	}

	if(Button_Room02)
	{
		Button_Room02->OnClicked.AddDynamic(this, &ULena_IntroSelectGameWidget::OnRoom02Animation);
	}

	if(Button_Room03)
	{
		Button_Room03->OnClicked.AddDynamic(this, &ULena_IntroSelectGameWidget::OnRoom03Animation);
	}

	if(Button_GoIntroWidget)
	{
		Button_GoIntroWidget->OnClicked.AddDynamic(this, &ULena_IntroSelectGameWidget::OnGoIntro);
	}
	
	return true;
}

void ULena_IntroSelectGameWidget::OnRoom01Animation()
{
	if(Button_Room01 && Button_Room02 && Button_Room03 && Button_GoIntroWidget)
	{
		Button_Room01->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		Button_Room02->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		Button_Room03->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		Button_GoIntroWidget->SetVisibility(ESlateVisibility::Hidden);
	}
	PlayAnimation(Animation_OnClickRoom01, 0.0f, 1);
	GetWorld()->GetTimerManager().SetTimer(Timer, this, &ULena_IntroSelectGameWidget::OnRoom01, 2.5f, false);
}

void ULena_IntroSelectGameWidget::OnRoom02Animation()
{
	if(Button_Room01 && Button_Room02 && Button_Room03 && Button_GoIntroWidget)
	{
		Button_Room01->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		Button_Room02->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		Button_Room03->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		Button_GoIntroWidget->SetVisibility(ESlateVisibility::Hidden);
	}
	PlayAnimation(Animation_OnClickRoom02, 0.0f, 1);
	GetWorld()->GetTimerManager().SetTimer(Timer, this, &ULena_IntroSelectGameWidget::OnRoom02, 2.5f, false);
}

void ULena_IntroSelectGameWidget::OnRoom03Animation()
{
	if(Button_Room01 && Button_Room02 && Button_Room03 && Button_GoIntroWidget)
	{
		Button_Room01->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		Button_Room02->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		Button_Room03->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		Button_GoIntroWidget->SetVisibility(ESlateVisibility::Hidden);
	}
	PlayAnimation(Animation_OnClickRoom03, 0.0f, 1);
	GetWorld()->GetTimerManager().SetTimer(Timer, this, &ULena_IntroSelectGameWidget::OnRoom03, 2.5f, false);
}

void ULena_IntroSelectGameWidget::OnRoom01()
{
	UGameplayStatics::OpenLevel(GetWorld(),TEXT("Room01_Lab"));
}

void ULena_IntroSelectGameWidget::OnRoom02()
{
	UGameplayStatics::OpenLevel(GetWorld(),TEXT("Room02_Outside"));
}

void ULena_IntroSelectGameWidget::OnRoom03()
{
	UGameplayStatics::OpenLevel(GetWorld(),TEXT("Room03_Tokyo"));
}

void ULena_IntroSelectGameWidget::OnGoIntro()
{
	RemoveFromParent();
	IntroPannel->PlayFadeIn();
}
