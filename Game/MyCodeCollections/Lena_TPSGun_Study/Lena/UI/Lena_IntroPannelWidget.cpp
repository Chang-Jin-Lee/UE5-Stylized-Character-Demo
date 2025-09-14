// Fill out your copyright notice in the Description page of Project Settings.


#include "Lena_IntroPannelWidget.h"

#include "Lena_IntroDescriptionWidget.h"
#include "Lena_IntroSelectGameWidget.h"
#include "Blueprint/UserWidgetBlueprint.h"
#include "Kismet/GameplayStatics.h"

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
		Button_Description->OnClicked.AddDynamic(this, &ULena_IntroPannelWidget::OnClickDesciptionTime);
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
	// SetVisibility(ESlateVisibility::Hidden);
	UBlueprintGeneratedClass* _widgetClass = LoadObject<UBlueprintGeneratedClass>(nullptr, TEXT("/Game/UI/HUD/IntroScene/WBP_IntroSelectGameWidget.WBP_IntroSelectGameWidget_C"));
	if (_widgetClass)
	{
		ULena_IntroSelectGameWidget* createdwidget = CreateWidget<ULena_IntroSelectGameWidget>(GetWorld(), _widgetClass);
		if(createdwidget)
		{
			createdwidget->SetInroPanel(this);
			createdwidget->AddToViewport();
		}
	}
}

void ULena_IntroPannelWidget::OnClickDesciptionTime()
{
	PlayAnimation(FadeOut, 0, 1);
	GetWorld()->GetTimerManager().SetTimer(Timer, this, &ULena_IntroPannelWidget::OnClickDesciption, 1.0f, false);
}

void ULena_IntroPannelWidget::OnClickDesciption()
{
	UBlueprintGeneratedClass* _widgetClass = LoadObject<UBlueprintGeneratedClass>(nullptr, TEXT("/Game/UI/HUD/IntroScene/WBP_IntroDesciptionWidget.WBP_IntroDesciptionWidget_C"));
	if (_widgetClass)
	{
		ULena_IntroDescriptionWidget* createdwidget = CreateWidget<ULena_IntroDescriptionWidget>(GetWorld(), _widgetClass);
		if(createdwidget)
		{
			createdwidget->SetInroPanel(this);
			createdwidget->AddToViewport();
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