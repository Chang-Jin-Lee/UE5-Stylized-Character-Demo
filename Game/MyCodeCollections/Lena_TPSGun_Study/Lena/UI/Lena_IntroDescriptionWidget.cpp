// Fill out your copyright notice in the Description page of Project Settings.


#include "Lena_IntroDescriptionWidget.h"

#include "Kismet/GameplayStatics.h"
#include "Lena/GameInstanceSubSystem/LenaRoom01Manager.h"
#include "Lena/GameInstanceSubSystem/LenaRoom03Manager.h"
#include "Lena/GameInstanceSubSystem/LenaUIManager.h"

bool ULena_IntroDescriptionWidget::Initialize()
{
	bool bSuccess = Super::Initialize();
	if(bSuccess == false) return false;
	
	if(Button_Room01)
	{
		Button_Room01->OnClicked.AddDynamic(this, &ULena_IntroDescriptionWidget::OnRoom01);
	}

	if(Button_Room02)
	{
		Button_Room02->OnClicked.AddDynamic(this, &ULena_IntroDescriptionWidget::OnRoom02);
	}

	if(Button_Room03)
	{
		Button_Room03->OnClicked.AddDynamic(this, &ULena_IntroDescriptionWidget::OnRoom03);
	}

	if(Button_GoIntroWidget)
	{
		Button_GoIntroWidget->OnClicked.AddDynamic(this, &ULena_IntroDescriptionWidget::OnGoIntro);
	}
	
	return true;
}

void ULena_IntroDescriptionWidget::OnRoom01()
{
	if(g_LenaUIManaerIsValid)
	{
		if(g_LenaRoom01ManagerIsValid)
		{
			g_LenaUIManager->LoadData(g_LenaRoom01Manager->GetLenaRoom01UIGuideData());
		}
		g_LenaUIManager->CreateGuideWidget(false);
	}
}

void ULena_IntroDescriptionWidget::OnRoom02()
{
	UGameplayStatics::OpenLevel(GetWorld(),TEXT("Room02_Outside"));
}

void ULena_IntroDescriptionWidget::OnRoom03()
{
	if(g_LenaUIManaerIsValid)
	{
		if(g_LenaRoom03ManagerIsValid)
		{
			g_LenaUIManager->LoadData(g_LenaRoom03Manager->GetLenaRoom03UIGuideData());
		}
		g_LenaUIManager->CreateGuideWidget(false);
	}
}

void ULena_IntroDescriptionWidget::OnGoIntro()
{
	RemoveFromParent();
	IntroPannel->PlayFadeIn();
}
