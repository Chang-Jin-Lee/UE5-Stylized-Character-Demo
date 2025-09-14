// Fill out your copyright notice in the Description page of Project Settings.


#include "VoteWidget.h"

#include "CJSlate/CJSlatePlayerController.h"
#include "Components/Button.h"

void UVoteWidget::NativeConstruct()
{
	Super::NativeConstruct();
	Button_Yes->OnClicked.AddDynamic(this, &UVoteWidget::Button_Yes_Clicked);
}

void UVoteWidget::Button_Yes_Clicked()
{
	ACJSlatePlayerController* pc = Cast<ACJSlatePlayerController>(GetOwningPlayer());
	if (pc)
		pc->Server_VoteCountAdd();
	SetVisibility(ESlateVisibility::Hidden);
}

void UVoteWidget::Button_No_Clicked()
{
	SetVisibility(ESlateVisibility::Hidden);
}
