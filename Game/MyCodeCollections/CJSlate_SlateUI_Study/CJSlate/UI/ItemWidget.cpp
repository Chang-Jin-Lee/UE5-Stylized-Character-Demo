// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemWidget.h"

#include "Components/TextBlock.h"

void UItemWidget::SetTextBlock(FString text)
{
	TextBlock->SetText(FText::FromString(text));
}
