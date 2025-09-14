// Fill out your copyright notice in the Description page of Project Settings.


#include "LenaUIManager.h"

#include "Kismet/GameplayStatics.h"
#include "Lena/GameInstance/LenaGameInstance.h"
#include "Lena/UI/Lena_InGameGuideWidget.h"

class ULena_InGameGuideWidget;

void ULenaUIManager::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
}

void ULenaUIManager::Deinitialize()
{
	Super::Deinitialize();
}

void ULenaUIManager::LoadData(TArray<FLenaUIGuide> _uIGuideData)
{
	if(_uIGuideData.IsEmpty() == false)
		LenaUIGuideData = _uIGuideData;
}

void ULenaUIManager::CreateGuideWidget(bool _isStart)
{
	if(UBlueprintGeneratedClass* _widgetclass = LoadObject<UBlueprintGeneratedClass>(nullptr, TEXT("/Game/UI/HUD/InGame/WBP_InGameGuideWidget.WBP_InGameGuideWidget_C")))
	{
		if(ULena_InGameGuideWidget* _widget = CreateWidget<ULena_InGameGuideWidget>(GetWorld(), _widgetclass))
		{
			_widget->AddToViewport();
			_widget->LoadData();
			_widget->SetbIsStartGuide(_isStart);
			if(_isStart)
				InputModeUI();
		}
	}
}

void ULenaUIManager::InputModeUI()
{
	if(ShooterPlayerController)
		ShooterPlayerController->InputModeUI();
	// APlayerController* controller = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	// if(controller)
	// {
	// 	controller->bShowMouseCursor = true;
	// 	controller->SetIgnoreLookInput(true);
	// 	controller->SetIgnoreMoveInput(true);
	// 	controller->SetInputMode(FInputModeGameAndUI());
	// }
}

void ULenaUIManager::InputModeGame()
{
	if(ShooterPlayerController)
		ShooterPlayerController->InputModeGame();
	// APlayerController* controller = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	// if(controller)
	// {
	// 	controller->bShowMouseCursor = false;
	// 	controller->SetIgnoreLookInput(false);
	// 	controller->SetIgnoreMoveInput(false);
	// 	controller->SetInputMode(FInputModeGameOnly());
	// }
}
