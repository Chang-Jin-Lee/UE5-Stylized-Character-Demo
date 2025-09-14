// Fill out your copyright notice in the Description page of Project Settings.


#include "Lena_IntroPannelWidget.h"

#include "Lena_InGameGuideWidget.h"
#include "Lena_IntroDescriptionWidget.h"
#include "Lena_IntroSelectGameWidget.h"
#include "Blueprint/UserWidgetBlueprint.h"
#include "Components/CanvasPanelSlot.h"
#include "Kismet/GameplayStatics.h"
#include "Lena/GameInstanceSubSystem/LenaRoom01Manager.h"
#include "Lena/GameInstanceSubSystem/LenaUIManager.h"

bool ULena_IntroPannelWidget::Initialize()
{
	bool bSuccess = Super::Initialize();
	if(bSuccess == false) return false;
	if(Button_GameStart)
	{
		Button_GameStart->OnClicked.AddDynamic(this, &ULena_IntroPannelWidget::OnGameStartTime);
	}
	if(Button_Description)
	{
		Button_Description->OnClicked.AddDynamic(this, &ULena_IntroPannelWidget::OnClickDesciption);
	}
	if(Button_Quit)
	{
		Button_Quit->OnClicked.AddDynamic(this, &ULena_IntroPannelWidget::OnQuitTime);
	}
	return true;
}

void ULena_IntroPannelWidget::OnGameStartTime()
{
	PlayAnimation(FadeOut, 0, 1);
	GetWorld()->GetTimerManager().SetTimer(Timer, this, &ULena_IntroPannelWidget::OnGameStart, 1.0f, false);
}

void ULena_IntroPannelWidget::OnGameStart()
{
	UGameplayStatics::OpenLevel(GetWorld(),TEXT("Room01_Lab"));
}

void ULena_IntroPannelWidget::OnClickDesciption()
{
	if (g_LenaUIManaerIsValid)
	{
		if(g_LenaRoom01ManagerIsValid)
			g_LenaUIManager->LoadData(g_LenaRoom01Manager->GetLenaRoom01UIGuideData());
		
		if(ULena_InGameGuideWidget* _widget = CreateWidget<ULena_InGameGuideWidget>(GetWorld(), InGameGuideWidgetCalss))
		{
			UCanvasPanelSlot* _widgetSlot = CanvasPanel_Root->AddChildToCanvas(_widget);
			_widgetSlot->SetAnchors(FAnchors(0.5f, 0.5f));
			_widgetSlot->SetAlignment(FVector2D(0.5f,0.5f));
			_widgetSlot->SetAutoSize(true);
			_widgetSlot->SetPosition(FVector2D(0,0));
			_widget->LoadData();
			_widget->SetbIsStartGuide(false);
		}
	}
}

void ULena_IntroPannelWidget::OnQuitTime()
{
	PlayAnimation(FadeOut, 0, 1);
	GetWorld()->GetTimerManager().SetTimer(Timer, this, &ULena_IntroPannelWidget::OnQuit, 1.0f, false);
}

void ULena_IntroPannelWidget::OnQuit()
{
	UWorld* World = GetWorld();
	APlayerController* PlayerController = World ? World->GetFirstPlayerController() : nullptr;
	if (PlayerController)
	{
		UKismetSystemLibrary::QuitGame(World, PlayerController, EQuitPreference::Quit, true);
	}
}

void ULena_IntroPannelWidget::PlayFadeIn()
{
	PlayAnimation(FadeIn,0,1);
}