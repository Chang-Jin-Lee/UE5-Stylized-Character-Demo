// Fill out your copyright notice in the Description page of Project Settings.


#include "InterationWidget.h"

#include "Components/TextBlock.h"

void UInterationWidget::SetTextBlock(FString text)
{
	if (text.IsEmpty() == false)
		TextBlock->SetText(FText::FromString(text));
	else
		TextBlock->SetText(FText::GetEmpty());
}
