// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/InventoryUIWidget.h"

#include "Blueprint/DragDropOperation.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Blueprint/WidgetTree.h"
#include "Characters/Base_Character.h"
#include "Components/Border.h"
#include "Components/BorderSlot.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/Image.h"
#include "Components/InventoryComponent.h"
#include "Components/TextBlock.h"
#include "Components/UniformGridPanel.h"
#include "Components/UniformGridSlot.h"
#include "Components/VerticalBox.h"
#include "Components/VerticalBoxSlot.h"
#include "Data/InventoryItem.h"
#include "Kismet/GameplayStatics.h"
#include "Widgets/Base_ItemSlotWidget.h"
#include "Widgets/ItemWidget.h"
#include "Framework/Application/SlateApplication.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Characters/NonPlayerCharacter.h"
#include "Characters/PlayerCharacter.h"
#include "Components/Widget.h"
#include "Widgets/InventoryHoverWidget.h"
#include "Widgets/SWidget.h"


TSharedRef<SWidget> UInventoryUIWidget::RebuildWidget()
{
	Super::RebuildWidget();
	
	// Canvas Panel 생성
	UCanvasPanel* RootPanel = WidgetTree->ConstructWidget<UCanvasPanel>(UCanvasPanel::StaticClass(), TEXT("RootCanvas"));
	WidgetTree->RootWidget = RootPanel;
	
	// DPI 스케일을 1.0으로 설정
	SetDesiredSizeInViewport(FVector2D(Base_Character->InventoryItemSlotSize.X * Columns, Base_Character->InventoryItemSlotSize.Y * Rows));
	SetRenderTransformPivot(FVector2D(0, 0));

	UVerticalBox* VerticalBox = WidgetTree->ConstructWidget<UVerticalBox>(UVerticalBox::StaticClass(), TEXT("VerticalBox"));

	UCanvasPanelSlot* VerticalBoxSlot = RootPanel->AddChildToCanvas(VerticalBox);
	VerticalBoxSlot->SetAnchors(FAnchors(0, 0));
	VerticalBoxSlot->SetAlignment(FVector2D(0, 0));
	VerticalBoxSlot->SetSize(FVector2D(Base_Character->InventoryItemSlotSize.X * Columns, Base_Character->InventoryItemSlotSize.Y * Rows));
	VerticalBoxSlot->SetPosition(FVector2D(0, 0));
	
	UTextBlock* TextBlock = WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass(), TEXT("TextBlock"));
	TextBlock->SetText(FText::FromString("Inventory"));
	FSlateFontInfo FontInfo = FCoreStyle::GetDefaultFontStyle("Regular", 28);
	TextBlock->SetFont(FontInfo); // 폰트 적용
	UVerticalBoxSlot* TextBlockSlot = VerticalBox->AddChildToVerticalBox(TextBlock);
	TextBlockSlot->SetHorizontalAlignment(HAlign_Center);

	UBorder* GridBorder = WidgetTree->ConstructWidget<UBorder>(UBorder::StaticClass(), TEXT("Border"));
	GridBorder->SetBrushColor(FLinearColor::Gray);
	UVerticalBoxSlot* BorderSlot = VerticalBox->AddChildToVerticalBox(GridBorder);
	BorderSlot->SetSize(ESlateSizeRule::Fill);

	// Uniform Grid Panel 생성
	InventoryGrid = WidgetTree->ConstructWidget<UUniformGridPanel>(UUniformGridPanel::StaticClass(), TEXT("InventoryGrid"));

	// Grid를 Canvas에 추가하고 중앙에 배치
	UVerticalBoxSlot* InventoryGridBoxSlot = VerticalBox->AddChildToVerticalBox(InventoryGrid);
	InventoryGridBoxSlot->SetSize(ESlateSizeRule::Fill);


	GridBorder->AddChild(InventoryGrid);

	// 인벤토리 초기화
	InitializeInventory();

	return Super::RebuildWidget();
}

void UInventoryUIWidget::InitializeInventory()
{
	UInventoryComponent* InventoryComponent;
	TArray<FInventoryItem> Items;
	if(Base_Character)
	{
		InventoryComponent = Base_Character->InventoryComponent;
		if(InventoryComponent)
		{
			Items = Base_Character->InventoryComponent->Items;
		}
	}
		
	if (InventoryGrid)
	{
		InventoryGrid->ClearChildren();
		FVector2D GridSize = InventoryGrid->GetDesiredSize();  // 그리드 크기를 가져옴
		FVector2D SlotSize = FVector2D(GridSize.X / Columns, GridSize.Y / Rows);  // 각 슬롯의 크기 계산

		for (int32 Row = 0; Row < Rows; ++Row)
		{
			for (int32 Column = 0; Column < Columns; ++Column)
			{
				FString SlotBorderName = FString::Printf(TEXT("SlotBorder_%d_%d"), Row, Column);
				FString ItemSlotWidgetName = FString::Printf(TEXT("ItemSlotWidget_%d_%d"), Row, Column);
				
				// 빈 슬롯으로 사용할 Border 생성
				UBorder* Border = WidgetTree->ConstructWidget<UBorder>(UBorder::StaticClass(), *SlotBorderName);
				Border->SetBrushColor(FLinearColor::Gray); // 그리드의 슬롯 색상 설정
				Border->SetHorizontalAlignment(HAlign_Center);
				Border->SetVerticalAlignment(VAlign_Center);
				
				UBase_ItemSlotWidget* Base_ItemSlotWidget = WidgetTree->ConstructWidget<UBase_ItemSlotWidget>(UBase_ItemSlotWidget::StaticClass(),*ItemSlotWidgetName);
				int32 index = (Row * Columns) + Column;
				if(Items.IsValidIndex(index) && !Items[index].IsEmpty())
				{
					Base_ItemSlotWidget->Itemdata = Items[index];
				}
				Base_ItemSlotWidget->SlotIndex = index;
				Base_ItemSlotWidget->Base_Character = Base_Character;
				Base_ItemSlotWidget->RebuildWidget();
				
				Border->AddChild(Base_ItemSlotWidget);
				
				UUniformGridSlot* BorderSlot = InventoryGrid->AddChildToUniformGrid(Border, Row, Column);
				BorderSlot->SetHorizontalAlignment(HAlign_Center);
				BorderSlot->SetVerticalAlignment(VAlign_Center);
				
			}
		}
	}
}

FReply UInventoryUIWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if (InMouseEvent.IsMouseButtonDown(EKeys::RightMouseButton))
	{
		FVector2D ClickPosition = InMouseEvent.GetScreenSpacePosition();
		FVector2D LocalPosition = InGeometry.AbsoluteToLocal(ClickPosition);
	
		// 슬롯의 크기를 계산합니다.
		FVector2D SlotSize = FVector2D(Base_Character->InventoryItemSlotSize.X, Base_Character->InventoryItemSlotSize.Y);

		// Row와 Column을 계산합니다.
		int32 ClickColumn = FMath::FloorToInt(LocalPosition.X / SlotSize.X);
		int32 ClickRow = FMath::FloorToInt(LocalPosition.Y / SlotSize.Y);
	
		UWidget* ClickedWidget = GetUWidgetFromPosition(FVector2D(ClickColumn, ClickRow));
		if(!ClickedWidget) return FReply::Handled();
	
		UBase_ItemSlotWidget* SourceItemSlotWidget = GetItemSlotWidgetFromParent(ClickedWidget);
		if(!SourceItemSlotWidget) return FReply::Handled();
	
		ABase_Character* SourceCharacter = SourceItemSlotWidget->Base_Character;
		if(!SourceCharacter) return FReply::Handled();

		int32 SourceIndex = SourceItemSlotWidget->SlotIndex;
		
		if(SourceItemSlotWidget->HoverWidget)
		{
			SourceItemSlotWidget->HoverWidget->RemoveFromParent();
		}

		if(SourceCharacter->IsA<ANonPlayerCharacter>())
		{
			ANonPlayerCharacter* npc = Cast<ANonPlayerCharacter>(SourceCharacter);
			ABase_Character* TargetCharacter = nullptr;
			if(npc)
			{
				TargetCharacter = npc->PlayerCharacer;
				if(TargetCharacter)
				{
					for(int32 i = 0 ; i < TargetCharacter->InventoryComponent->Items.Num(); ++i)
					{
						if(TargetCharacter->InventoryComponent->Items[i].IsEmpty())
						{
							TargetCharacter->InventoryComponent->Items[i] = SourceCharacter->InventoryComponent->Items[SourceIndex];
							SourceCharacter->InventoryComponent->Items[SourceIndex].Empty();
							break;
						}
					}
				}
				TargetCharacter->InventoryReload();
				SourceCharacter->InventoryReload();
			}
		}
		else if (SourceCharacter->IsA<APlayerCharacter>())
		{
			APlayerCharacter* pc = Cast<APlayerCharacter>(SourceCharacter);
			ABase_Character* TargetCharacter = nullptr;
			if(pc)
			{
				TargetCharacter = pc->NonPlayerCharacter;
				if(TargetCharacter)
				{
					for(int32 i = 0 ; i < TargetCharacter->InventoryComponent->Items.Num(); ++i)
					{
						if(TargetCharacter->InventoryComponent->Items[i].IsEmpty())
						{
							TargetCharacter->InventoryComponent->Items[i] = SourceCharacter->InventoryComponent->Items[SourceIndex];
							SourceCharacter->InventoryComponent->Items[SourceIndex].Empty();
							break;
						}
					}
					TargetCharacter->InventoryReload();
					SourceCharacter->InventoryReload();
				}
				else
				{
					pc->DrawWeaponFromInventory(SourceIndex);
				}
			}
		}
		
		
		return FReply::Handled();
	}
	
	return Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
}


bool UInventoryUIWidget::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	FVector2D DropPosition = InDragDropEvent.GetScreenSpacePosition();
	FVector2D LocalPosition = InGeometry.AbsoluteToLocal(DropPosition);
	
		// 슬롯의 크기를 계산합니다.
	FVector2D SlotSize = FVector2D(Base_Character->InventoryItemSlotSize.X, Base_Character->InventoryItemSlotSize.Y);

	// Row와 Column을 계산합니다.
	int32 DroppedColumn = FMath::FloorToInt(LocalPosition.X / SlotSize.X);
	int32 DroppedRow = FMath::FloorToInt(LocalPosition.Y / SlotSize.Y);
	
	UWidget* DroppedWidget = GetUWidgetFromPosition(FVector2D(DroppedColumn, DroppedRow));
	if(!DroppedWidget) return false;
	
	UBase_ItemSlotWidget* TargetItemSlotWidget = GetItemSlotWidgetFromParent(DroppedWidget);
	if(!TargetItemSlotWidget) return false;
	
	ABase_Character* TargetCharacter = TargetItemSlotWidget->Base_Character;
	if(!TargetCharacter) return false;

	if(InOperation->Payload)
	{
		UBase_ItemSlotWidget* SourceItemSlotWidget = Cast<UBase_ItemSlotWidget>(InOperation->Payload);
		
		if(SourceItemSlotWidget)
		{
			if(!SourceItemSlotWidget->Itemdata.IsEmpty())
			{
				ABase_Character* SourceCharacter = SourceItemSlotWidget->Base_Character;
				int32 TargetIndex = TargetItemSlotWidget->SlotIndex;
				int32 SourceIndex = SourceItemSlotWidget->SlotIndex;
				
				if(SourceCharacter->IsA<ANonPlayerCharacter>())
				{
					// 구매하기
					if(!SourceCharacter->InventoryComponent->Items[SourceIndex].IsEmpty())
					{
						TargetCharacter->InventoryComponent->Items[TargetIndex] = SourceCharacter->InventoryComponent->Items[SourceIndex];
						SourceCharacter->InventoryComponent->Items[SourceIndex].Empty();
					}
				}
				else if(SourceCharacter->IsA<APlayerCharacter>())
				{
					// 판매하기
					if(!SourceCharacter->InventoryComponent->Items[SourceIndex].IsEmpty())
					{
						TargetCharacter->InventoryComponent->Items[TargetIndex] = SourceCharacter->InventoryComponent->Items[SourceIndex];
						SourceCharacter->InventoryComponent->Items[SourceIndex].Empty();
					}
				}

				if(SourceItemSlotWidget->HoverWidget->IsInViewport())
				{
					SourceItemSlotWidget->HoverWidget->RemoveFromParent();
				}
				
				TargetCharacter->InventoryReload();
				SourceCharacter->InventoryReload();
			}
		}
	}
	return false;
}

UBase_ItemSlotWidget* UInventoryUIWidget::GetItemSlotWidgetFromParent(UWidget* ParentWidget)
{
	UBase_ItemSlotWidget* ItemSlotWidget = nullptr;
	
	if(ParentWidget)
	{
		UPanelWidget* PanelParent = Cast<UPanelWidget>(ParentWidget);
		if (PanelParent)
		{
			for (int32 i = 0; i < PanelParent->GetChildrenCount(); i++)
			{
				UWidget* ChildWidget = PanelParent->GetChildAt(i);
				ItemSlotWidget = Cast<UBase_ItemSlotWidget>(ChildWidget);
				break;
			}
		}
	}

	return ItemSlotWidget;
}

UWidget* UInventoryUIWidget::GetUWidgetFromPosition(FVector2D Position)
{
	// Dropped 위치의 위젯을 가져옵니다.
	UWidget* DroppedWidget = nullptr;

	for (UWidget* Child : InventoryGrid->GetAllChildren())
	{
		UUniformGridSlot* GridSlot = Cast<UUniformGridSlot>(Child->Slot);
		if (GridSlot && GridSlot->GetRow() == Position.Y && GridSlot->GetColumn() == Position.X)
		{
			DroppedWidget = Child;
			break;
		}
	}

	return DroppedWidget;
}

