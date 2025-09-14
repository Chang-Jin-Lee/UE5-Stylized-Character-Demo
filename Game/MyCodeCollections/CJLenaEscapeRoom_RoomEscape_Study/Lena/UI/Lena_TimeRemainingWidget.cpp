// Fill out your copyright notice in the Description page of Project Settings.


#include "Lena_TimeRemainingWidget.h"

#include "Components/TextBlock.h"

void ULena_TimeRemainingWidget::SetDescriptionText(FString _string)
{
	TextBlock_Description->SetText(FText::FromString(_string));
}

void ULena_TimeRemainingWidget::VisibleDescriptionText()
{
	if(TextBlock_Description)
		TextBlock_Description->SetVisibility(ESlateVisibility::Visible);
}

void ULena_TimeRemainingWidget::HiddenDescriptionText()
{
	if(TextBlock_Description)
		TextBlock_Description->SetVisibility(ESlateVisibility::Hidden);
}