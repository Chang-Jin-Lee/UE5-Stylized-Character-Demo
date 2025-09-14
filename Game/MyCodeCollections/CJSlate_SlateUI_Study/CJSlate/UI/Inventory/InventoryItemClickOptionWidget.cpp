// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryItemClickOptionWidget.h"

#include "InventoryWidgetSlot.h"
#include "CJSlate/CJSlatePlayerController.h"
#include "Components/Button.h"

class ACJSlatePlayerController;

void UInventoryItemClickOptionWidget::NativeConstruct()
{
	Super::NativeConstruct();

	Button_Drop->OnClicked.AddDynamic(this, &UInventoryItemClickOptionWidget::Drop);
	Button_Close->OnClicked.AddDynamic(this, &UInventoryItemClickOptionWidget::Close);
}

void UInventoryItemClickOptionWidget::Drop()
{
	if (ItemActorInfo.ItemID != -1)
	{
		ACJSlatePlayerController* pc = Cast<ACJSlatePlayerController>(GetOwningPlayer());
		if (pc)
			pc->Server_ItemDrop(ItemActorInfo);
		ParentSlot->RemoveFromParent();
		RemoveFromParent();
	}
}

void UInventoryItemClickOptionWidget::Close()
{
	RemoveFromParent();
}
