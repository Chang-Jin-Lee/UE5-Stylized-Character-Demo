// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "Data/InventoryItem.h"
#include "Base_Character.generated.h"

class UInventoryComponent;
class UInventoryUIWidget;

UCLASS(config=Game)
class ABase_Character : public ACharacter
{
	GENERATED_BODY()
	
public:
	ABase_Character();
	
	UPROPERTY(EditAnywhere, Category="Inventory")
	UInventoryComponent* InventoryComponent;
	
	FVector2D InventoryItemSlotSize = FVector2D(FItemSlotSize::Width, FItemSlotSize::Height);
	FVector2D InventoryWidgetPosition;
	FVector2D InventoryGridCount;
	void InventoryReload();
	void CloseInventory();
	void CreateAndShowInventory();
	void ShowInventory();
	
protected:
	// To add mapping context
	virtual void BeginPlay();
	
	UInventoryUIWidget* PlayerUIWidget;
	void OpenInventory();
	void InputModeUI();
	void InputModeGame();
};

