// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Base_ItemSlotWidget.h"

#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Blueprint/WidgetTree.h"
#include "Components/Border.h"
#include "Components/BorderSlot.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/Image.h"
#include "Components/SizeBox.h"
#include "Kismet/GameplayStatics.h"
#include "Widgets/InventoryHoverWidget.h"

TSharedRef<SWidget> UBase_ItemSlotWidget::RebuildWidget()
{
	SetDesiredSizeInViewport(Base_Character->InventoryItemSlotSize);
	SetRenderTransformPivot(FVector2D(0, 0));
	
	USizeBox* SizeBox = WidgetTree->ConstructWidget<USizeBox>(USizeBox::StaticClass(), TEXT("SizeBox"));
	SizeBox->SetHeightOverride(Base_Character->InventoryItemSlotSize.X);
	SizeBox->SetWidthOverride(Base_Character->InventoryItemSlotSize.Y);
	WidgetTree->RootWidget = SizeBox;

	
	UBorder* Border = WidgetTree->ConstructWidget<UBorder>(UBorder::StaticClass(), TEXT("Border"));
	Border->SetHorizontalAlignment(HAlign_Fill);
	Border->SetVerticalAlignment(VAlign_Fill);
	Border->SetPadding(FMargin(2,2,2,2));
	Border->SetBrushColor(FLinearColor::Gray);

	SizeBox->AddChild(Border);

	Image = WidgetTree->ConstructWidget<UImage>(UImage::StaticClass(), TEXT("Image"));
	Image->SetBrushFromTexture(Itemdata.ItemImage);
	
	Border->AddChild(Image);
	
	return Super::RebuildWidget();
}

FReply UBase_ItemSlotWidget::NativeOnPreviewMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if(Itemdata.IsEmpty())
	{
		return FReply::Handled();
	}
	
	if (InMouseEvent.IsMouseButtonDown(EKeys::LeftMouseButton))
	{
		return UWidgetBlueprintLibrary::DetectDragIfPressed(InMouseEvent, this, EKeys::LeftMouseButton).NativeReply;
	}
	
	return Super::OnMouseButtonDown(InGeometry, InMouseEvent).NativeReply;
}

void UBase_ItemSlotWidget::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)
{
	if(Itemdata.IsEmpty())
	{
		OutOperation = nullptr;
		return;
	}
	
	UDragDropOperation* DragDropOp = NewObject<UDragDropOperation>();
	DragDropOp->DefaultDragVisual = this;
	DragDropOp->Payload = this;
	OutOperation = DragDropOp;
}

FReply UBase_ItemSlotWidget::NativeOnMouseMove(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if(Itemdata.IsEmpty())
	{
		return FReply::Handled();
	}
	FVector2D MousePosition = InMouseEvent.GetScreenSpacePosition();
	FVector2D LocalPosition = InGeometry.AbsoluteToLocal(MousePosition);

	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if(PlayerController)
	{
		FVector2D Position;
		PlayerController->GetMousePosition(Position.X, Position.Y);
		HoverWidget->SetPositionInViewport(Position + LocalPosition + FVector2d(1,1), true);
	}
	
	return FReply::Handled();
}


void UBase_ItemSlotWidget::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if(!Itemdata.IsEmpty())
	{
		FVector2D MousePosition = InMouseEvent.GetScreenSpacePosition();
		FVector2D LocalPosition = InGeometry.AbsoluteToLocal(MousePosition);
		
		HoverWidget = CreateWidget<UInventoryHoverWidget>(GetWorld(),UInventoryHoverWidget::StaticClass());
		HoverWidget->ItemData = Itemdata;
		HoverWidget->AddToViewport();
	
		APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
		if(PlayerController)
		{
			FVector2D Position;
			PlayerController->GetMousePosition(Position.X, Position.Y);
			HoverWidget->SetPositionInViewport(Position + LocalPosition + FVector2d(1,1), true);
		}
	}
}

void UBase_ItemSlotWidget::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	if(Itemdata.IsEmpty())
	{
		return;
	}
	HoverWidget->RemoveFromParent();
}

void UBase_ItemSlotWidget::NativeDestruct()
{
	if(HoverWidget)
	{
		HoverWidget->RemoveFromParent();
	}
}
