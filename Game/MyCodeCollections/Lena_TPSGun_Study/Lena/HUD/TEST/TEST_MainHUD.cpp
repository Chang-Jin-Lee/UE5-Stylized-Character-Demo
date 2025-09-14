// Fill out your copyright notice in the Description page of Project Settings.


#include "TEST_MainHUD.h"

#include "Blueprint/UserWidget.h"
#include "CoreFramework/GameInstance/CJGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Lena/UI/TEST/TEST_MainPanelWidget.h"

class ULena_IntroPannelWidget;
struct FLevelDataRow;
class UCJGameInstance;

void ATEST_MainHUD::BeginPlay()
{
	Super::BeginPlay();

	FString MapName = GetWorld()->GetMapName();
	FString CleanMapName = MapName.Mid(GetWorld()->StreamingLevelsPrefix.Len());

	if (UCJGameInstance* _GameInstance = Cast<UCJGameInstance>(UGameplayStatics::GetGameInstance(GetWorld())))
	{
		for(FLevelDataRow _leveldata : _GameInstance->LevelData)
		{
			FString LevelName = _leveldata.LevelName;
			FString HUDPath = _leveldata.HUD;

			UE_LOG(LogTemp, Warning, TEXT("Loading HUD for Level: %s with HUD path: %s"), *LevelName, *HUDPath);

			if(LevelName == CleanMapName)
			{
				UBlueprintGeneratedClass* _widgetClass = LoadObject<UBlueprintGeneratedClass>(nullptr, *HUDPath);
                
				if (_widgetClass)
				{
					UTEST_MainPanelWidget* createdwidget = CreateWidget<UTEST_MainPanelWidget>(GetWorld(), _widgetClass);
					if(createdwidget)
					{
						createdwidget->AddToViewport();
						UE_LOG(LogTemp, Warning, TEXT("Widget successfully added to viewport."));
						WidgetPanel = createdwidget;
					}
					else
					{
						UE_LOG(LogTemp, Error, TEXT("Failed to create widget from class."));
					}
				}
				else
				{
					UE_LOG(LogTemp, Error, TEXT("Failed to load widget class from path: %s"), *HUDPath);
				}
			}
		}
	}
}
