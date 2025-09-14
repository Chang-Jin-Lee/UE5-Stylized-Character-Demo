// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "ManagerRegistry/Data/LenaUIGuide.h"
#include "Lena_InGameGuideWidget.generated.h"

class UButton;
/**
 * 
 */
UCLASS()
class LENA_API ULena_InGameGuideWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual bool Initialize() override;
	void LoadData();
	
	UPROPERTY(meta=(BindWidgetOptional))
	TObjectPtr<UImage> Image_Description;

	UPROPERTY(meta=(BindWidgetOptional))
	TObjectPtr<UTextBlock> TextBlock_Description;
	
	UPROPERTY(meta=(BindWidgetOptional))
	TObjectPtr<UButton> Button_Prev;
	
	UPROPERTY(meta=(BindWidgetOptional))
	TObjectPtr<UButton> Button_Next;

	UPROPERTY(meta=(BindWidgetOptional))
	TObjectPtr<UButton> Button_Exit;

	UPROPERTY()
	TArray<FLenaUIGuide> UIGuidesData;
	
	UFUNCTION()
	void OnButtonNext();
	UFUNCTION()
	void OnButtonPrev();
	UFUNCTION()
	void OnButtonExit();

	FORCEINLINE bool GetbIsStartGuide() { return bIsStartGuide; }
	FORCEINLINE void SetbIsStartGuide(bool _in) { bIsStartGuide = _in; }

private:
	int32 currentIndex;
	bool bIsStartGuide;
};
