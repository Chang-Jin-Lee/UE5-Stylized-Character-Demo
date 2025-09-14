// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryWidgetSlot.h"

#include "InventoryHoverWidget.h"
#include "InventoryItemClickOptionWidget.h"
#include "InventoryWidget.h"
#include "ScreenPass.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Blueprint/WidgetTree.h"
#include "CJSlate/CJSlatePlayerController.h"
#include "CJSlate/Item/ItemActor.h"
#include "Components/Button.h"
#include "Components/CanvasPanel.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/UniformGridPanel.h"
#include "GameplayTagContainer.h"

void UInventoryWidgetSlot::NativeConstruct()
{
	Super::NativeConstruct();
	// Button_ItemButton->OnClicked.AddDynamic(this, &UInventoryWidgetSlot::ItemClick);
}

void UInventoryWidgetSlot::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseEnter(InGeometry, InMouseEvent);
	if (ItemActorInfo.ItemID != - 1)
	{
		UE::Slate::FDeprecateVector2DResult Position = InMouseEvent.GetScreenSpacePosition();
		CreateHoverWidget(FVector2D(Position.X, Position.Y));
	}
}

void UInventoryWidgetSlot::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseLeave(InMouseEvent);
	if (ItemActorInfo.ItemID != - 1)
	{
		DestroyHoverWidget();
	}
}

FReply UInventoryWidgetSlot::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if (ItemActorInfo.ItemID != - 1)
	{
		DestroyClickOptionWidget();
		UE::Slate::FDeprecateVector2DResult Position = InMouseEvent.GetScreenSpacePosition();
		CreateItemClickOptionWidget(FVector2D(Position.X , Position.Y));
	}
	return Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
}

void UInventoryWidgetSlot::Load(UTexture2D* Texture, FText Count)
{
	Image_ItemImage->SetBrushFromTexture(Texture, true);
	TextBlock_Count->SetText(Count);
}

void UInventoryWidgetSlot::Destroy()
{
	if (UInventoryHoverWidgetptr)
		UInventoryHoverWidgetptr->RemoveFromParent();
	if (UInventoryItemClickOptionWidgetptr)
		UInventoryItemClickOptionWidgetptr->RemoveFromParent();
}

void UInventoryWidgetSlot::CreateHoverWidget(FVector2D Position)
{
	UUserWidget* widget = CreateWidget(this, UInventoryHoverWidgetClass);
	if (widget)
	{
		widget->AddToViewport();
		widget->SetPositionInViewport(Position);
		UInventoryHoverWidgetptr = Cast<UInventoryHoverWidget>(widget);
		if (UInventoryHoverWidgetptr)
		{
			UInventoryHoverWidgetptr->Load(ItemActorInfo);
		}
	}
}

void UInventoryWidgetSlot::DestroyHoverWidget()
{
	if (UInventoryHoverWidgetptr)
		UInventoryHoverWidgetptr->RemoveFromParent();
}

void UInventoryWidgetSlot::DestroyClickOptionWidget()
{
	if (ParentWidget)
	{
		for (auto slot : ParentWidget->InventoryWidgetSlots)
		{
			if (slot->UInventoryItemClickOptionWidgetptr)
				slot->UInventoryItemClickOptionWidgetptr->RemoveFromParent();
		}
	}
}

void UInventoryWidgetSlot::CreateItemClickOptionWidget(FVector2D Position)
{
	UUserWidget* widget = CreateWidget(this, UInventoryItemClickOptionWidgetClass);
	if (widget)
	{
		widget->AddToViewport();
		widget->SetPositionInViewport(Position);
		UInventoryItemClickOptionWidgetptr = Cast<UInventoryItemClickOptionWidget>(widget);
		UInventoryItemClickOptionWidgetptr->ItemActorInfo = ItemActorInfo;
		UInventoryItemClickOptionWidgetptr->ParentSlot = this;
	}
}
