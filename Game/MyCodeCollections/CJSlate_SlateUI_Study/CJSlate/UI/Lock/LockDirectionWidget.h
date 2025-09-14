// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LockDirectionWidget.generated.h"

class UButton;
class ALockActor;
/**
 * 
 */
UCLASS()
class CJSLATE_API ULockDirectionWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	void LoadData(ALockActor* LockActor,  int32 ButtonIndex);
	
	UPROPERTY(meta=(BindWidgetOptional))
	UButton* Button_Direction;

	UPROPERTY(Replicated)
	ALockActor* m_ATargetLockActor;	// 자물쇠 액터의 Beginplay에서 해당 레퍼런스를 얻어옴.
	
	UPROPERTY(Replicated)
	int32 m_iRotationButtonIndex;	// 자물쇠 액터의 Beginplay에서 돌릴 휠의 인덱스를 얻어옴.

	UFUNCTION()
	void RotateLockButton();
};
