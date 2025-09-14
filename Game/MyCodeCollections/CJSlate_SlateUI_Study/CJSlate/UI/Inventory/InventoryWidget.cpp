// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryWidget.h"

#include "InventoryWidgetSlot.h"
#include "CJSlate/CJSlatePlayerController.h"
#include "CJSlate/Item/ItemActor.h"
#include "Components/CanvasPanel.h"
#include "Components/UniformGridPanel.h"

class UInventoryWidgetSlot;
class ACJSlatePlayerController;
struct FItemActorInfo;

void UInventoryWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UInventoryWidget::LoadData()
{
	UniformGridPanel_Item->ClearChildren();
	
	TArray<FItemActorInfo> Iteminfos;	
	ACJSlatePlayerController* pc = Cast<ACJSlatePlayerController>(GetOwningPlayer());
	if (pc)
		Iteminfos = pc->m_fItemActorInfo;
	int32 cur = 0;
	for (int32 i = 0 ; i < ItemRow; i++)
	{
		for (int32 j = 0; j < ItemCol; j++)
		{
			UUserWidget* widget = CreateWidget(this, UInventoryWidgetSlotClass);
			if (widget)
			{
				if (UInventoryWidgetSlot* slotwidget = Cast<UInventoryWidgetSlot>(widget))
				{
					if (Iteminfos.Num() > cur)
					{
						FItemActorInfo TargetItem = Iteminfos[cur++];
						slotwidget->Load(TargetItem.ItemImage, FText::FromString(FString::FromInt(TargetItem.ItemCount)));
						slotwidget->ItemActorInfo = TargetItem;
						slotwidget->ParentWidget = this;
						InventoryWidgetSlots.Add(slotwidget);
					}
					UniformGridPanel_Item->AddChildToUniformGrid(slotwidget, i, j);
				}
				else
				{
					UniformGridPanel_Item->AddChildToUniformGrid(widget, i, j);
				}
			}
		}
	}
}
