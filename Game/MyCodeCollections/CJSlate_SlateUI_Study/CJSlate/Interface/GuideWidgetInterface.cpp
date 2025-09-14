// Fill out your copyright notice in the Description page of Project Settings.


#include "GuideWidgetInterface.h"

#include "Components/Border.h"
#include "Components/CircularThrobber.h"
#include "Components/TextBlock.h"


// Add default functionality here for any IMoveInterface functions that are not pure virtual.
void IGuideWidgetInterface::PopUp(FText text)
{
	if (text.IsEmpty() == false)
		GetDescriptionText()->SetText(text);
	else
		GetDescriptionText()->SetText(FText::GetEmpty());
	GetGuideBackGroundBorder()->SetVisibility(ESlateVisibility::Visible);
	GetLoadingBar()->SetVisibility(ESlateVisibility::Visible);
	GetDescriptionText()->SetVisibility(ESlateVisibility::Visible);
}

void IGuideWidgetInterface::PopDown()
{
	GetGuideBackGroundBorder()->SetVisibility(ESlateVisibility::Hidden);
	GetLoadingBar()->SetVisibility(ESlateVisibility::Hidden);
	GetDescriptionText()->SetVisibility(ESlateVisibility::Hidden);
}