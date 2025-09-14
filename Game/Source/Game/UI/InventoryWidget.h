#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"

/*
 * @brief : 인벤토리 슬롯(정사각형 칸)
 */
class SInventorySlotWidget : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SInventorySlotWidget) {}
		SLATE_ARGUMENT(FVector2D, SlotSize)
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs)
	{
		const FVector2D Size = InArgs._SlotSize;
		ChildSlot
		[
			SNew(SBox)
			.WidthOverride(Size.X)
			.HeightOverride(Size.Y)
			[
				SNew(SBorder)
				.Padding(FMargin(2.f))
				.BorderImage(&FCoreStyle::Get().GetWidgetStyle<FButtonStyle>("Button").Normal)
				[
					SNew(STextBlock).Justification(ETextJustify::Center).Text(FText::FromString(TEXT("")))
				]
			]
		];
	}
};

/*
 * @brief : 인벤토리 그리드(UI)
 */
class SInventoryWidget : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SInventoryWidget) {}
		SLATE_ARGUMENT(int32, Rows)
		SLATE_ARGUMENT(int32, Cols)
		SLATE_ARGUMENT(FVector2D, SlotSize)
		SLATE_ARGUMENT(FMargin, SlotPadding)
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);

private:
	int32 Rows = 4;
	int32 Cols = 6;
	FVector2D SlotSize = FVector2D(64.f, 64.f);
	FMargin SlotPadding = FMargin(4.f);
};


