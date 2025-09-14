// Fill out your copyright notice in the Description page of Project Settings.


#include "Lena_Room02HUD.h"
#include "Blueprint/UserWidget.h"
#include "CoreFramework/GameInstance/CJGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Lena/UI/Lena_InGameWIdget.h"

void ALena_Room02HUD::BeginPlay()
{
	Super::BeginPlay();

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
				UBlueprintGeneratedClass* _widgetClass = LoadObject<UBlueprintGeneratedClass>(nullptr, *HUDPath);
				if (_widgetClass)
				{
					ULena_InGameWIdget* createdwidget = CreateWidget<ULena_InGameWIdget>(GetWorld(), _widgetClass);
					if(createdwidget)
					{
						createdwidget->AddToViewport();
						createdwidget->HiddenInit();
						createdwidget->HideEnemyLeftWidget();
					}
					else
						UE_LOG(LogTemp, Warning, TEXT("createdwidget :: CreateWidgetFail"));
				}
			}
		}
	}
}
