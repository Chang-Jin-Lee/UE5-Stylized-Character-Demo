#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"

class AGameCharacter;
class UAbilityComponent;

/*
 * @brief : 플레이어 디버그/토글 오버레이 (Slate)
 */
class SPlayerOverlayWidget : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SPlayerOverlayWidget) {}
		SLATE_ARGUMENT(TWeakObjectPtr<AGameCharacter>, Player)
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);

private:
	FReply OnToggleCustomDepth();
	FText GetTransformText() const;
	TSharedRef<SWidget> BuildAbilityList() const;
	EActiveTimerReturnType HandleTick(double InCurrentTime, float InDeltaTime);

private:
	TWeakObjectPtr<AGameCharacter> Player;
	TSharedPtr<class SVerticalBox> AbilityBox;
	TMap<FName, TSharedPtr<class STextBlock>> AbilityTexts;
};


