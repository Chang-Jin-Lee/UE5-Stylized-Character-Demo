// Fill out your copyright notice in the Description page of Project Settings.


#include "Lena_InGameMenuPannel.h"

#include "Kismet/GameplayStatics.h"
#include "Lena/GameInstanceSubSystem/LenaUIManager.h"

bool ULena_InGameMenuPannel::Initialize()
{
	bool bSuccess = Super::Initialize();
	if(bSuccess == false) return false;
	if(Button_Restart)
	{
		Button_Restart->OnClicked.AddDynamic(this, &ULena_InGameMenuPannel::OnGameRestart);
	}
	if(Button_GoIntroPannel)
	{
		Button_GoIntroPannel->OnClicked.AddDynamic(this, &ULena_InGameMenuPannel::OnGameGoIntroPannel);
	}
	if(Button_Hint)
	{
		Button_Hint->OnClicked.AddDynamic(this, &ULena_InGameMenuPannel::OnGameHint);
	}
	if(Button_Guide)
	{
		Button_Guide->OnClicked.AddDynamic(this, &ULena_InGameMenuPannel::OnGameGuide);
	}
	if(Button_Quit)
	{
		Button_Quit->OnClicked.AddDynamic(this, &ULena_InGameMenuPannel::OnGameQuit);
	}
	if(Button_Exit)
	{
		Button_Exit->OnClicked.AddDynamic(this, &ULena_InGameMenuPannel::OnGameExit);
	}
	return true;
}

void ULena_InGameMenuPannel::OnGameRestart()
{
	FString MapName = GetWorld()->GetMapName();
	FString CleanMapName = MapName.Mid(GetWorld()->StreamingLevelsPrefix.Len());
	UGameplayStatics::OpenLevel(GetWorld(), FName(*CleanMapName));
}

void ULena_InGameMenuPannel::OnGameGoIntroPannel()
{
	UGameplayStatics::OpenLevel(GetWorld(), "IntroScene");
}

void ULena_InGameMenuPannel::OnGameHint()
{
	
}

void ULena_InGameMenuPannel::OnGameGuide()
{
	if(g_LenaUIManaerIsValid)
	{
		g_LenaUIManager->CreateGuideWidget(false);
	}
}

void ULena_InGameMenuPannel::OnGameQuit()
{
	UWorld* World = GetWorld();
	APlayerController* PlayerController = World ? World->GetFirstPlayerController() : nullptr;
	if (PlayerController)
	{
		UKismetSystemLibrary::QuitGame(World, PlayerController, EQuitPreference::Quit, true);
	}
}
void ULena_InGameMenuPannel::OnGameExit()
{
	if(g_LenaUIManaerIsValid)
	{
		g_LenaUIManager->InputModeGame();
		g_LenaUIManager->GetShooterPlayerController()->SetbMenuShowed(!g_LenaUIManager->GetShooterPlayerController()->GetbMenuShowed());
	}
	RemoveFromParent();
}

