// Fill out your copyright notice in the Description page of Project Settings.


#include "Lena_Room03HUD.h"
#include "Blueprint/UserWidget.h"
#include "CoreFramework/GameInstance/CJGameInstance.h"
#include "Engine/BlueprintGeneratedClass.h"
#include "Kismet/GameplayStatics.h"
#include "Lena/GameInstanceSubSystem/LenaRoom03Manager.h"
#include "Lena/GameInstanceSubSystem/LenaUIManager.h"
#include "Lena/UI/Lena_InGameWIdget.h"

void ALena_Room03HUD::BeginPlay()
{
	Super::BeginPlay();

	if(g_LenaRoom03ManagerIsValid)
	{
		if(ABase_Character* Character = Cast<ABase_Character>(UGameplayStatics::GetPlayerCharacter(GetWorld(),0)))
		{
			g_LenaRoom03Manager->SetCharacter(Character);
		}
		if(AShooterPlayerController* controller = Cast<AShooterPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(),0)))
		{
			g_LenaRoom03Manager->SetPlayerController(controller);
		}
	}
	
	if(g_LenaUIManaerIsValid)
	{
		if(g_LenaRoom03ManagerIsValid)
			g_LenaUIManager->LoadData(g_LenaRoom03Manager->GetLenaRoom03UIGuideData());
		if(ABase_Character* Character = Cast<ABase_Character>(UGameplayStatics::GetPlayerCharacter(GetWorld(),0)))
		{
			g_LenaUIManager->SetBaseCharacter(Character);
		}
		if(AShooterPlayerController* controller = Cast<AShooterPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(),0)))
		{
			g_LenaUIManager->SetShooterPlayerController(controller);
		}
	}

	FString MapName = GetWorld()->GetMapName();
	FString CleanMapName = MapName.Mid(GetWorld()->StreamingLevelsPrefix.Len());

	if (UCJGameInstance* _GameInstance = ::Cast<UCJGameInstance>(UGameplayStatics::GetGameInstance(GetWorld())))
	{
		for(FLevelDataRow _leveldata : _GameInstance->LevelData)
		{
			FString LevelName = _leveldata.LevelName;
			FString HUDPath = _leveldata.HUD;
			if(LevelName == CleanMapName)
			{
				if (UBlueprintGeneratedClass* _widgetClass = LoadObject<UBlueprintGeneratedClass>(nullptr, *HUDPath))
				{
					if(ULena_InGameWIdget* createdwidget = CreateWidget<ULena_InGameWIdget>(GetWorld(), _widgetClass))
					{
						createdwidget->AddToViewport();
						createdwidget->HiddenInit();
						createdwidget->CreateGuideWidget();
						if(g_LenaUIManaerIsValid)
							g_LenaUIManager->SetInGameWidget(createdwidget);
						break;
					}
				}
			}
		}
	}
}
