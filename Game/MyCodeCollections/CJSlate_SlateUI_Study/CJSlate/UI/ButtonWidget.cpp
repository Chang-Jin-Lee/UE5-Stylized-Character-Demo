// Fill out your copyright notice in the Description page of Project Settings.


#include "ButtonWidget.h"

#include "Components/TextBlock.h"

void UButtonWidget::SetTextBlock(FString text)
{
	TextBlock->SetText(FText::FromString(text));
}
