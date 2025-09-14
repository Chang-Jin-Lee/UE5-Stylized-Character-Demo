// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Lena/Characters/AI_Character.h"
#include "Lena_InGameAIHealthBarWidget.generated.h"

UCLASS()
class LENA_API ULena_InGameAIHealthBarWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual bool Initialize() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	void HiddenInit();
	
	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<UProgressBar> Progressbar_Healthbar;

	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<UTextBlock> TextBlock_Health;
	
	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<UTextBlock> TextBlock_AmmoLeft;

	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<UTextBlock> TextBlock_AmmoAll;

	void SetHealthPercent(float _input);
	void GetHealth(TObjectPtr<AAI_Character> ShooterCharacter);
	void GetAmmoInInventory(TObjectPtr<AAI_Character> ShooterCharacter);
	void GetAmmoInGun(TObjectPtr<AAI_Character> ShooterCharacter);
};