// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CJSlate/Item/ItemActor.h"
#include "InventoryWidgetSlot.generated.h"

class UInventoryWidget;
class UCanvasPanel;
class UInventoryItemClickOptionWidget;
class UInventoryHoverWidget;
class UTextBlock;
class UImage;
class UButton;
/**
 * 
 */
UCLASS()
class CJSLATE_API UInventoryWidgetSlot : public UUserWidget
{
	GENERATED_BODY()
public:

	virtual void NativeConstruct() override;
	virtual void NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override;
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

	// 아이템 슬롯에서 보이는 모습을 로드하거나 초기화하는 함수들
	// 만약 정보가 추가되면 여기에 매개변수가 추가되어야함.
	void Load(UTexture2D* Texture, FText Count);
	void Destroy();
	
	UPROPERTY(meta = (BindWidgetOptional))
	UImage* Image_ItemImage;

	UPROPERTY(meta = (BindWidgetOptional))
	UButton* Button_ItemButton;

	UPROPERTY(meta = (BindWidgetOptional))
	UTextBlock* TextBlock_Count;

	// InventoryWidget으로부터 전달 받은 아이템 정보.
	UPROPERTY()
	FItemActorInfo ItemActorInfo;
	
	// 마우스를 아이템 위에 올리면 나타날 호버 설명
	UPROPERTY()
	TObjectPtr<UInventoryHoverWidget> UInventoryHoverWidgetptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	TSubclassOf<UInventoryHoverWidget> UInventoryHoverWidgetClass;
	
	UFUNCTION()
	void CreateHoverWidget(FVector2D Position);

	UFUNCTION()
	void DestroyHoverWidget();

	void DestroyClickOptionWidget();

	// 아이템을 클릭하면 나올 옵션 
	UPROPERTY()
	TObjectPtr<UInventoryItemClickOptionWidget> UInventoryItemClickOptionWidgetptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	TSubclassOf<UInventoryItemClickOptionWidget> UInventoryItemClickOptionWidgetClass;

	UFUNCTION()
	void CreateItemClickOptionWidget(FVector2D Position);

	UPROPERTY()
	UInventoryWidget* ParentWidget;
};