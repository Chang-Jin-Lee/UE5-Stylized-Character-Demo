// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PickUpDefense/Units/PlayerUnit/BaseUnit.h"
#include "OptionMenu.generated.h"

class UButton;
class UVerticalBox;
/**
 * 
 */
UCLASS()
class PICKUPDEFENSE_API UOptionMenu : public UUserWidget
{
	GENERATED_BODY()

	virtual TSharedRef<SWidget> RebuildWidget() override;

public:
	UPROPERTY()
	ABaseUnit* TargetUnit = nullptr;

private:
	template<typename T>
	UVerticalBox* AddVerticalBox(T* ParentWidget, FName VerticalBoxName);

	template<typename T>
	UButton* AddButton(T* ParentWidget, FName ButtonBlockName, FName ButtonName, float FontSize);

	UFUNCTION()
	void OnButtonClicked();
};
