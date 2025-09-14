#include "Game/UI/InventoryWidget.h"
#include "Widgets/Layout/SGridPanel.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Layout/SBorder.h"
#include "Widgets/Text/STextBlock.h"

void SInventoryWidget::Construct(const FArguments& InArgs)
{
	Rows = InArgs._Rows == 0 ? 4 : InArgs._Rows;
	Cols = InArgs._Cols == 0 ? 6 : InArgs._Cols;
	if (InArgs._SlotSize != FVector2D::ZeroVector) SlotSize = InArgs._SlotSize;
	if (InArgs._SlotPadding.GetDesiredSize().X >= 0) SlotPadding = InArgs._SlotPadding;

	TSharedRef<SGridPanel> Grid = SNew(SGridPanel);
	for (int32 R = 0; R < Rows; ++R)
	{
		for (int32 C = 0; C < Cols; ++C)
		{
			Grid->AddSlot(C, R)
			.Padding(SlotPadding)
			[
				SNew(SInventorySlotWidget)
				.SlotSize(SlotSize)
			];
		}
	}

	ChildSlot
	[
		SNew(SBorder)
		.Padding(FMargin(6.f))
		[
			Grid
		]
	];
}


