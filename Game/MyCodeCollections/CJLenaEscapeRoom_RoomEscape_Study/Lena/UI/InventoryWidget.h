// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/ScrollBox.h"
#include "Components/SizeBox.h"
#include "Components/VerticalBox.h"
#include "Lena/Characters/Base_Character.h"
#include "InventoryWidget.generated.h"

class UTextBlock;
class UInventorySlotWidget;
/**
 * 
 */
UCLASS()
class LENA_API UInventoryWidget : public UUserWidget
{
	GENERATED_BODY()

	virtual void NativeConstruct() override;
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
	
public:
	UPROPERTY(meta=(BindWidgetOptional))
	TObjectPtr<UScrollBox> GroundScrollBox;

	UPROPERTY(meta=(BindWidgetOptional))
	TObjectPtr<USizeBox> InventorySlot;

	UPROPERTY(meta=(BindWidgetOptional))
	TObjectPtr<UScrollBox> ItemScrollBox;

	UPROPERTY(meta=(BindWidgetOptional))
	TObjectPtr<USizeBox> CharacterSlot;

	UPROPERTY(meta=(BindWidgetOptional))
	TObjectPtr<USizeBox> EquipmentSlot;

	UPROPERTY(meta=(BindWidgetOptional))
	TObjectPtr<UVerticalBox> EquipmentBox;

	UPROPERTY(meta=(BindWidgetOptional))
	TObjectPtr<UTextBlock> TextBlock_Weight;
	
	UPROPERTY(EditAnywhere, Category="Drag And Drop")
	FVector2D InventoryAreaMin = FVector2D(430,50);
	UPROPERTY(EditAnywhere, Category="Drag And Drop")
	FVector2D InventoryAreaMax = FVector2D(730,1030);
	UPROPERTY(EditAnywhere, Category="Drag And Drop")
	FVector2D GroundAreaMin = FVector2D(80,50);
	UPROPERTY(EditAnywhere, Category="Drag And Drop")
	FVector2D GroundAreaMax = FVector2D(380,1030);
	UPROPERTY(EditAnywhere, Category="Drag And Drop")
	FVector2D WeaponAreaMin = FVector2D(1380,50);
	UPROPERTY(EditAnywhere, Category="Drag And Drop")
	FVector2D WeaponAreaMax = FVector2D(1880,1030);

	// UFUNCTION()
	// void RemoveGroundSlot();
	//
	// UFUNCTION()
	// void AddGroundSlot();

	void SetTextBlock_Weight(float _currentWeight, float _maxWeight);

	UFUNCTION()
	void BuildInventorySlot();

	UFUNCTION()
	void BuildGroundSlot();

	UFUNCTION()
	void BuildEquipmentSlot();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<ABase_Character> Base_Character;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class AShooterPlayerController* PlayerController;

	FORCEINLINE void SetBaseCharacter(TObjectPtr<ABase_Character> _ch) {Base_Character = _ch;}
	FORCEINLINE TObjectPtr<ABase_Character> GetBaseCharacter() { return Base_Character; }
	FORCEINLINE void SetPlayerController(TObjectPtr<AShooterPlayerController> _pc) {PlayerController = _pc;}
	FORCEINLINE TObjectPtr<AShooterPlayerController> GetPlayerController() { return PlayerController;}
	
private:
	// InventoryArea
	bool IsInInventoryArea(FVector2D LocalPosition);
	void SetInventorySlot(UInventorySlotWidget* SlotWidget, EInventorySlotType SlotType);

	//GroundArea
	bool IsInGroundArea(FVector2D LocalPosition);
	void SetGroundSlot(UInventorySlotWidget* SlotWidget, EInventorySlotType SlotType);

	// WeaponSlot
	bool IsInWeaponArea(FVector2D LocalPosition);
	void SetEquipSlot(int32 SlotIndex, UInventorySlotWidget* SlotWidget, EInventorySlotType SlotType);
	int32 GetEquipSlotIndex(FVector2D LocalPosition);
};
