#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"

/*
 * @brief : 간단한 상점 UI (목업)
 */
class SShopWidget : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SShopWidget) {}
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs)
	{
		ChildSlot
		[
			SNew(STextBlock).Text(FText::FromString(TEXT("Shop (Placeholder)")))
		];
	}
};


