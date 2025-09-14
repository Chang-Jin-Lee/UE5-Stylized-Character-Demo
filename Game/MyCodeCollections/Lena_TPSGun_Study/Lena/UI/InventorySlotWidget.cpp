// Fill out your copyright notice in the Description page of Project Settings.

#include "Lena/UI/InventorySlotWidget.h"

#include "InventoryHoverWidget.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Kismet/GameplayStatics.h"

class UBlueprintGeneratedClass;

FReply UInventorySlotWidget::NativeOnPreviewMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if(ItemData.ItemID.IsEmpty()) return FReply::Handled();
	if (InMouseEvent.IsMouseButtonDown(EKeys::LeftMouseButton))
	{
		return UWidgetBlueprintLibrary::DetectDragIfPressed(InMouseEvent, this, EKeys::LeftMouseButton).NativeReply;
	}
	return Super::OnMouseButtonDown(InGeometry, InMouseEvent).NativeReply;
}

void UInventorySlotWidget::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)
{
	if(ItemData.ItemID.IsEmpty()) return;
	UDragDropOperation* DragDropOp = NewObject<UDragDropOperation>();
	DragDropOp->DefaultDragVisual = this;
	DragDropOp->Payload = this;
	OutOperation = DragDropOp;
}

void UInventorySlotWidget::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseEnter(InGeometry, InMouseEvent);

	if(ItemData.ItemID.IsEmpty()) return;
	UBlueprintGeneratedClass* _widgetClass = LoadObject<UBlueprintGeneratedClass>(nullptr, TEXT("/Game/UI/HUD/WBP_InventorySlotHover.WBP_InventorySlotHover_C"));
	if (_widgetClass)
	{
		UInventoryHoverWidget* createdwidget = CreateWidget<UInventoryHoverWidget>(GetWorld(), _widgetClass);
		if(createdwidget)
		{
			createdwidget->SetItemData(ItemData);
			SetHoverWidget(createdwidget);
			createdwidget->PlayFadeIn();
			createdwidget->AddToViewport();
			createdwidget->LoadData();
			MovdHoverWidgetPosition();
		}
	}
}

void UInventorySlotWidget::MovdHoverWidgetPosition()
{
	if(HoverWidget)
	{
		float locationX, locationY;
		UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetMousePosition(locationX, locationY);
		HoverWidget->SetPositionInViewport(FVector2d(locationX+25, locationY+25), true);
	}
}

FReply UInventorySlotWidget::NativeOnMouseMove(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	MovdHoverWidgetPosition();
	return Super::NativeOnMouseMove(InGeometry, InMouseEvent);
}

void UInventorySlotWidget::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseLeave(InMouseEvent);
	if(HoverWidget)
		HoverWidget->PlayFadeOut();
}
