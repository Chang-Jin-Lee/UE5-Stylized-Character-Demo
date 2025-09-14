// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ShopPanelSlot.generated.h"

class UTextBlock;
class UBorder;
class UHorizontalBox;
class UVerticalBox;
class UImage;
/**
 * 
 */
UCLASS()
class PICKUPDEFENSE_API UShopPanelSlot : public UUserWidget
{
	GENERATED_BODY()

	virtual TSharedRef<SWidget> RebuildWidget() override;
	virtual void NativeConstruct() override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Card Info")
	FString CardName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Card Info")
	FString Synergy1Text;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Card Info")
	FString Synergy2Text;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Card Info")
	FString Synergy3Text;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Card Info")
	FString PriceText;

	UPROPERTY()
	FString ImageDir;

	UPROPERTY()
	FString MeshDir;

	UPROPERTY()
	FLinearColor BorderColor;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Card Info")
	UTextBlock* Synergy1;

	UPROPERTY()
	UTextBlock* Synergy2;

	UPROPERTY()
	UTextBlock* Synergy3;

	UPROPERTY()
	UTextBlock* Name;

	UPROPERTY()
	UTextBlock* Price;

	UPROPERTY()
	UImage* Image;
	
	UPROPERTY()
	UBorder* BorderInVerticalBox;

	void UpdateCardInfo();

private:
	template<typename T>
	UVerticalBox* AddVerticalBox(T* ParentWidget, FName VerticalBoxName);
	
	template<typename T>
	UHorizontalBox* AddHorizontalBox(T* ParentWidget, FName HorizontalBoxName);

	template<typename T>
	UBorder* AddBlockBox(T* ParentWidget, FName BorderName, FLinearColor Color, FMargin Padding, EHorizontalAlignment HorizontalAlignment, EVerticalAlignment VerticalAlignment);

	template<typename T>
	UTextBlock* AddTextBlock(T* ParentWidget, FName TextName, FText Text, float FontSize, float BlockSize, EHorizontalAlignment HorizontalAlignment, EVerticalAlignment VerticalAlignment, FMargin Margin);

	virtual void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation) override;
	virtual FReply NativeOnPreviewMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnDragCancelled(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
};
