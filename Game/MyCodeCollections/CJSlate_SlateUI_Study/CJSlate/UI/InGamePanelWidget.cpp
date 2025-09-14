// Fill out your copyright notice in the Description page of Project Settings.


#include "InGamePanelWidget.h"

#include "InterationWidget.h"
#include "Blueprint/WidgetTree.h"
#include "Components/Border.h"
#include "Components/CanvasPanel.h"
#include "Components/TextBlock.h"
#include "Inventory/InventoryHoverWidget.h"
#include "Inventory/InventoryItemClickOptionWidget.h"
#include "Inventory/InventoryWidget.h"
#include "Inventory/InventoryWidgetSlot.h"

void UInGamePanelWidget::NativeConstruct()
{
	Super::NativeConstruct();
	InventoryWidget->SetVisibility(ESlateVisibility::Hidden);
	InteractDone();
	PopDown();
}

void UInGamePanelWidget::Interact(FString ObjectName)
{
	InteractionWidget->SetTextBlock(ObjectName);
	if (GetVisibility() == ESlateVisibility::Hidden)
		SetVisibility(ESlateVisibility::Visible);
}

void UInGamePanelWidget::InteractDone()
{
	InteractionWidget->SetTextBlock(FString(""));
	if (GetVisibility() == ESlateVisibility::Visible)
		SetVisibility(ESlateVisibility::Hidden);
}

void UInGamePanelWidget::PopUp(FText text)
{
	IGuideWidgetInterface::PopUp(text);
	GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
	GetWorld()->GetTimerManager().SetTimer(TimerHandle,this, &UInGamePanelWidget::PopDown, 5.0f);
}

void UInGamePanelWidget::PopDown()
{
	IGuideWidgetInterface::PopDown();
}

void UInGamePanelWidget::SetText_TextBlock_Guide(FText _value)
{
	if (_value.IsEmpty() == false)
		TextBlock_Description->SetText(_value);
}

void UInGamePanelWidget::PlayAnimation_GuideFadeIn()
{
	Border_GuideBackGround->SetVisibility(ESlateVisibility::Visible);
	PlayAnimation(WidgetAnimation_GuideFadein);
	GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
	GetWorld()->GetTimerManager().SetTimer(TimerHandle,this, &UInGamePanelWidget::PlayAnimation_GuideFadeOut, 6.0f);
}

void UInGamePanelWidget::PlayAnimation_GuideFadeOut()
{
	PlayAnimation(WidgetAnimation_GuideFadeOut);
	Border_GuideBackGround->SetVisibility(ESlateVisibility::Hidden);
}

void UInGamePanelWidget::TimeOut()
{
	
}
