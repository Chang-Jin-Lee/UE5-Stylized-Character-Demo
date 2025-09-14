// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/EditableTextBox.h"
#include "NameInputWidget.generated.h"

class UBorder;
/**
 * 
 */
UCLASS()
class CHANGJINCODESTORY_API UNameInputWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UEditableTextBox* NameTextBox;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UButton* ConfirmButton;

protected:
	virtual TSharedRef<SWidget> RebuildWidget() override;

private:
	UBorder* CreateTitleSlot(FText Title);
	UBorder* CreateTextInputBoxSlot(FText HintText);
	UBorder* CreateButtonSlot();
};
