// Fill out your copyright notice in the Description page of Project Settings.


#include "Lena_Room01HUD.h"

#include "Blueprint/UserWidget.h"
#include "CoreFramework/GameInstance/CJGameInstance.h"
#include "Engine/BlueprintGeneratedClass.h"
#include "Kismet/GameplayStatics.h"
#include "Lena/GameInstanceSubSystem/LenaRoom01Manager.h"
#include "Lena/GameInstanceSubSystem/LenaUIManager.h"
#include "Lena/UI/Lena_InGameWIdget.h"

void ALena_Room01HUD::BeginPlay()
{
	Super::BeginPlay();

	if(g_LenaRoom01ManagerIsValid)
	{
		if(ABase_Character* Character = Cast<ABase_Character>(UGameplayStatics::GetPlayerCharacter(GetWorld(),0)))
		{
			g_LenaRoom01Manager->SetCharacter(Character);
		}
		if(AShooterPlayerController* controller = Cast<AShooterPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(),0)))
		{
			g_LenaRoom01Manager->SetPlayerController(controller);
		}
	}
	if(g_LenaUIManaerIsValid)
	{
		if(g_LenaRoom01ManagerIsValid)
			g_LenaUIManager->LoadData(g_LenaRoom01Manager->GetLenaRoom01UIGuideData());
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
						createdwidget->HideEnemyLeftWidget();
						if(g_LenaRoom01ManagerIsValid)
							g_LenaRoom01Manager->SetInGameWidget(createdwidget);
						break;
					}
				}
			}
		}
	}
}
