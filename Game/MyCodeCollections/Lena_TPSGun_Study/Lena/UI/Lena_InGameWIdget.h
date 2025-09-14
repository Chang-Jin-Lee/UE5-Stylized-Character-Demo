// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Lena_TimeRemainingWidget.h"
#include "Components/TextBlock.h"
#include "Blueprint/UserWidget.h"
#include "Components/CanvasPanel.h"
#include "Components/HorizontalBox.h"
#include "Components/Image.h"
#include "Components/ProgressBar.h"
#include "Components/SizeBox.h"
#include "ManagerRegistry/Data/LenaUIGuide.h"
#include "Lena_InGameWIdget.generated.h"

UCLASS()
class LENA_API ULena_InGameWIdget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual bool Initialize() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	void LoadData(TArray<FLenaUIGuide> _lenaUIGuide);
	void HiddenInit();
	UPROPERTY(meta=(BindWidgetOptional))
	TObjectPtr<UCanvasPanel> CanvasPanel_MainPannel;
	
	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<UProgressBar> Progressbar_Healthbar;

	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<UTextBlock> TextBlock_Health;
	
	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<UTextBlock> TextBlock_AmmoLeft;

	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<UTextBlock> TextBlock_AmmoAll;

	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<UTextBlock> TextBlock_EnemyLeft;

	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<UTextBlock> TextBlock_EnemyAll;

	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<UHorizontalBox> HorizontalBox_EnemyLeft;
	
	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<USizeBox> SizeBox_EnemyLeft;
	
	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<ULena_TimeRemainingWidget> Lena_TimeRemainingWidget;

	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<UImage> Image_Ammo;

	void SetHealthPercent(float _input);
	void SetDescriptionText(FString _string);
	void VisibleDescriptionText();
	void HiddenDescriptionText();
	void VisibleDescriptionTextRoom03Win();
	void UpdateDescriptionTextRoom03Win(int32 _time);
	void OpenIntroScene();
	void CreateGuideWidget();
	void HideEnemyLeftWidget();
	void GetAmmoInInventory();
	void GetAmmoInGun();
	void GetEnemyInMap();
	void GetHealth();

	UPROPERTY()
	TArray<FLenaUIGuide> LenaUIGuideData;
};
