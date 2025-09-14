// Fill out your copyright notice in the Description page of Project Settings.

#include "Lena/UI/InventoryWidget.h"

#include "Blueprint/DragDropOperation.h"
#include "Lena/Characters/Base_Character.h"
#include "Lena/Controller/ShooterPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Lena/GameInstanceSubSystem/LenaUIManager.h"
#include "Lena/UI/InventoryEquipmentSlotWidget.h"
#include "Lena/UI/InventoryGroundSlotWidget.h"
#include "Lena/UI/InventoryItemSlotWidget.h"
#include "Lena/UI/InventorySlotWidget.h"

class UBlueprintGeneratedClass;

void UInventoryWidget::NativeConstruct()
{
	// BuildInventorySlot();
	// BuildGroundSlot();
	// BuildEquipmentSlot();
}

bool UInventoryWidget::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	FVector2D DropPosition = InDragDropEvent.GetScreenSpacePosition();
	FVector2D LocalPosition = InGeometry.AbsoluteToLocal(DropPosition);

	if(InOperation->Payload)
	{
		if(UInventorySlotWidget* SlotWidget = Cast<UInventorySlotWidget>(InOperation->Payload))
		{
			if(IsInGroundArea(LocalPosition))
			{
				if(SlotWidget->IsA<UInventoryItemSlotWidget>())
				{
					SetGroundSlot(SlotWidget, EInventorySlotType::ISt_Inventory);
				}
				else if(SlotWidget->IsA<UInventoryEquipmentSlotWidget>())
				{
					SetGroundSlot(SlotWidget, EInventorySlotType::ISt_Equipment);
				}
			}
			else if(IsInInventoryArea(LocalPosition))
			{
				if(SlotWidget->IsA<UInventoryGroundSlotWidget>())
				{
					SetInventorySlot(SlotWidget, EInventorySlotType::ISt_Ground);
				}
				else if(SlotWidget->IsA<UInventoryEquipmentSlotWidget>())
				{
					SetInventorySlot(SlotWidget, EInventorySlotType::ISt_Equipment);
				}
			}
			else if (IsInWeaponArea(LocalPosition))
			{
				if(SlotWidget->IsA<UInventoryItemSlotWidget>())
				{
					SetEquipSlot(GetEquipSlotIndex(LocalPosition),SlotWidget, EInventorySlotType::ISt_Inventory);
				}
				else if(SlotWidget->IsA<UInventoryGroundSlotWidget>())
				{
					SetEquipSlot(GetEquipSlotIndex(LocalPosition),SlotWidget, EInventorySlotType::ISt_Ground);
				}
			}
			BuildGroundSlot();
			BuildInventorySlot();
			BuildEquipmentSlot();
			if(SlotWidget->GetHoverWidget())
			{
				SlotWidget->GetHoverWidget()->RemoveFromParent();
			}
		}
	}
	return Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);
}

void UInventoryWidget::SetTextBlock_Weight(float _currentWeight, float _maxWeight)
{
	FString _string = FString::Printf(TEXT("Weight: %f / %f"), _currentWeight, _maxWeight);
	if(TextBlock_Weight)
		TextBlock_Weight->SetText(FText::FromString(_string));
}

void UInventoryWidget::BuildInventorySlot()
{
	if(ItemScrollBox)
		ItemScrollBox->ClearChildren();

	if(Base_Character == nullptr) return;

	float weight = 0;
	if(Base_Character->InventoryComponent.IsValid())
	{
		for(FInventoryItem _item : Base_Character->InventoryComponent.Get()->Items)
		{
			weight += _item.Weight;
			UBlueprintGeneratedClass* _widgetClass = LoadObject<UBlueprintGeneratedClass>(nullptr, TEXT("/Game/UI/HUD/WBP_InventorySlot.WBP_InventorySlot_C"));
			if (_widgetClass)
			{
				UInventoryItemSlotWidget* createdwidget = CreateWidget<UInventoryItemSlotWidget>(GetWorld(), _widgetClass);
				if(createdwidget)
				{
					createdwidget->SetItemData(_item);
					createdwidget->SetBaseCharacter(Base_Character);
					createdwidget->SetInventoryWidgetMain(this);
					createdwidget->AddToViewport();
					createdwidget->LoadData();
					if(ItemScrollBox)
						ItemScrollBox->AddChild(createdwidget);
				}
			}
		}
	}
	FString _string = FString::Printf(TEXT("Weight: %.2f"), weight);
	if(TextBlock_Weight)
		TextBlock_Weight->SetText(FText::FromString(_string));
}

void UInventoryWidget::BuildGroundSlot()
{
	if(GroundScrollBox)
		GroundScrollBox->ClearChildren();
	
	if(Base_Character == nullptr) return;

	if(Base_Character->GroundItemsComponent.IsValid())
	{
		for(FInventoryItem _item : Base_Character->GroundItemsComponent.Get()->Items)
		{
			UBlueprintGeneratedClass* _widgetClass = LoadObject<UBlueprintGeneratedClass>(nullptr, TEXT("/Game/UI/HUD/WBP_GroundSlot.WBP_GroundSlot_C"));
			if (_widgetClass)
			{
				UInventoryGroundSlotWidget* createdwidget = CreateWidget<UInventoryGroundSlotWidget>(GetWorld(), _widgetClass);
				if(createdwidget)
				{
					createdwidget->SetItemData(_item);
					createdwidget->SetBaseCharacter(Base_Character);
					createdwidget->SetInventoryWidgetMain(this);
					createdwidget->AddToViewport();
					createdwidget->LoadData();
					if(GroundScrollBox)
						GroundScrollBox->AddChild(createdwidget);
				}
			}
		}
	}
}

void UInventoryWidget::BuildEquipmentSlot()
{
	if(EquipmentBox)
		EquipmentBox->ClearChildren();
	
	if(Base_Character == nullptr) return;

	if(Base_Character->WeaponSlotsComponent.IsValid())
	{
		if(Base_Character->WeaponSlotsComponent.Get()->Items.Num() > 0)
		{
			for(int32 i = 0; i < Base_Character->WeaponSlotsComponent.Get()->Items.Num(); i++)
			{
				FInventoryItem _item = Base_Character->WeaponSlotsComponent.Get()->Items[i];
				UBlueprintGeneratedClass* _widgetClass = LoadObject<UBlueprintGeneratedClass>(nullptr, TEXT("/Game/UI/HUD/WBP_EquipMentSlot.WBP_EquipMentSlot_C"));
				if (_widgetClass)
				{
					UInventoryEquipmentSlotWidget* createdwidget = CreateWidget<UInventoryEquipmentSlotWidget>(GetWorld(), _widgetClass);
					if(createdwidget)
					{
						createdwidget->SetItemData(_item);
						createdwidget->SetBaseCharacter(Base_Character);
						createdwidget->SetInventoryWidgetMain(this);
						createdwidget->SetEquipmentSlotIndex(i);
						createdwidget->AddToViewport();
						createdwidget->LoadData();
						if(EquipmentBox)
							EquipmentBox->AddChild(createdwidget);
					}
				}
			}
		}
	}
}


bool UInventoryWidget::IsInGroundArea(FVector2D LocalPosition)
{
	return LocalPosition.X > GroundAreaMin.X && LocalPosition.X < GroundAreaMax.X && LocalPosition.Y > GroundAreaMin.Y && LocalPosition.Y < GroundAreaMax.Y;
}

bool UInventoryWidget::IsInInventoryArea(FVector2D LocalPosition)
{
	return LocalPosition.X > InventoryAreaMin.X && LocalPosition.X < InventoryAreaMax.X && LocalPosition.Y > InventoryAreaMin.Y && LocalPosition.Y < InventoryAreaMax.Y;
}

bool UInventoryWidget::IsInWeaponArea(FVector2D LocalPosition)
{
	
	return LocalPosition.X > WeaponAreaMin.X && LocalPosition.X < WeaponAreaMax.X && LocalPosition.Y > WeaponAreaMin.Y && LocalPosition.Y < WeaponAreaMax.Y;
}

void UInventoryWidget::SetGroundSlot(UInventorySlotWidget* SlotWidget, EInventorySlotType SlotType)
{
	FInventoryItem ItemData = SlotWidget->GetItemData();
	SlotWidget->GetBaseCharacter()->DropItem(ItemData, SlotType);
	if(PlayerController)
	{
		PlayerController->CheckPickUpItemSweep();
	}
}

void UInventoryWidget::SetInventorySlot(UInventorySlotWidget* SlotWidget, EInventorySlotType SlotType)
{
	FInventoryItem ItemData = SlotWidget->GetItemData();
	SlotWidget->GetBaseCharacter()->PickupItem(ItemData.ItemActor, SlotType);
	if(PlayerController)
	{
		PlayerController->CheckPickUpItemSweep();
	}
}

void UInventoryWidget::SetEquipSlot(int32 SlotIndex, UInventorySlotWidget* SlotWidget, EInventorySlotType SlotType)
{
	FInventoryItem ItemData = SlotWidget->GetItemData();
	if(ItemData.ItemType != EItemType::It_Equipment) return;
	Base_Character->EquipItem(ItemData, SlotType, SlotIndex);
	if(PlayerController)
	{
		PlayerController->CheckPickUpItemSweep();
	}
}

int32 UInventoryWidget::GetEquipSlotIndex(FVector2D LocalPosition)
{
	const int32 NumSlots = 4;
	const float SlotHeight = (WeaponAreaMax.Y - WeaponAreaMin.Y) / NumSlots;

	// LocalPosition.Y를 이용해 슬롯 인덱스를 계산합니다.
	int32 SlotIndex = FMath::Clamp(static_cast<int32>((LocalPosition.Y - WeaponAreaMin.Y) / SlotHeight), 0, NumSlots - 1);

	return SlotIndex;
}

