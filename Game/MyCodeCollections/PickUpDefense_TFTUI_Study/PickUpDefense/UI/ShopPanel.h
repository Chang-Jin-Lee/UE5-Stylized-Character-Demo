// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ShopPanel.generated.h"

class UShopPanelSlot;
struct FCardData;
/**
 * 
 */
UCLASS()
class PICKUPDEFENSE_API UShopPanel : public UUserWidget
{
	GENERATED_BODY()

	virtual TSharedRef<SWidget> RebuildWidget() override;

private:
	template<typename T>
	UShopPanelSlot* AddCardToChild(T* ParentWidget, FName BorderName);

	template<typename T>
	void AddSideBox(T* ParentWidget, FName SlotName, float Size);

	template<typename T>
	void AddMiddleBox(T* ParentWidget, FName SlotName);

	void InitializeCard(UShopPanelSlot* Panel, const FCardData& CardData);
};

