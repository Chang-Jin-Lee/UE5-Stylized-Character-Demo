#include "Game/UI/PlayerOverlayWidget.h"
#include "Game/GameCharacter.h"
#include "Game/Ability/AbilityComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Widgets/Text/STextBlock.h"
#include "Widgets/Input/SButton.h"
#include "Widgets/Layout/SScrollBox.h"
#include "Widgets/Layout/SBorder.h"

void SPlayerOverlayWidget::Construct(const FArguments& InArgs)
{
	Player = InArgs._Player;

	SAssignNew(AbilityBox, SVerticalBox);

	ChildSlot
	[
		SNew(SBorder)
		.Padding(FMargin(6.f))
		[
			SNew(SVerticalBox)
			+ SVerticalBox::Slot().AutoHeight().HAlign(HAlign_Left)
			[
				SNew(SButton)
				.Text(FText::FromString(TEXT("CustomDepth")))
				.ContentPadding(FMargin(6.f,2.f))
				.OnClicked(this, &SPlayerOverlayWidget::OnToggleCustomDepth)
			]
			+ SVerticalBox::Slot().AutoHeight().HAlign(HAlign_Left)
			[
				SNew(STextBlock)
				.Text(this, &SPlayerOverlayWidget::GetTransformText)
			]
			+ SVerticalBox::Slot().AutoHeight().HAlign(HAlign_Left)
			[
				AbilityBox.ToSharedRef()
			]
		]
	];

	RegisterActiveTimer(0.25, FWidgetActiveTimerDelegate::CreateSP(this, &SPlayerOverlayWidget::HandleTick));
}

FReply SPlayerOverlayWidget::OnToggleCustomDepth()
{
	if (AGameCharacter* P = Player.Get())
	{
		if (USkeletalMeshComponent* Mesh = P->GetMesh())
		{
			const bool bNow = !Mesh->bRenderCustomDepth;
			Mesh->SetRenderCustomDepth(bNow);
		}
	}
	return FReply::Handled();
}

FText SPlayerOverlayWidget::GetTransformText() const
{
	if (const AGameCharacter* P = Player.Get())
	{
		const FVector L = P->GetActorLocation();
		const FRotator R = P->GetActorRotation();
		return FText::FromString(FString::Printf(TEXT("Loc: (%.1f, %.1f, %.1f)  Rot: (%.1f, %.1f, %.1f)"), L.X, L.Y, L.Z, R.Roll, R.Pitch, R.Yaw));
	}
	return FText::FromString(TEXT("No Player"));
}

TSharedRef<SWidget> SPlayerOverlayWidget::BuildAbilityList() const
{
	TSharedRef<SVerticalBox> VBox = SNew(SVerticalBox);
	if (const AGameCharacter* P = Player.Get())
	{
		if (const UAbilityComponent* Abil = P->FindComponentByClass<UAbilityComponent>())
		{
			for (const TPair<FName, float>& Pair : Abil->GetAttributes())
			{
				TSharedPtr<STextBlock> Text;
				Text = SNew(STextBlock).Text(FText::FromString(FString::Printf(TEXT("%s : %.1f"), *Pair.Key.ToString(), Pair.Value)));
				VBox->AddSlot().AutoHeight()[ Text.ToSharedRef() ];
			}
		}
	}
	return VBox;
}

EActiveTimerReturnType SPlayerOverlayWidget::HandleTick(double InCurrentTime, float InDeltaTime)
{
	if (const AGameCharacter* P = Player.Get())
	{
		if (const UAbilityComponent* Abil = P->FindComponentByClass<UAbilityComponent>())
		{
			// Update existing entries, create missing
			for (const TPair<FName, float>& Pair : Abil->GetAttributes())
			{
				if (TSharedPtr<STextBlock>* Found = AbilityTexts.Find(Pair.Key))
				{
					(*Found)->SetText(FText::FromString(FString::Printf(TEXT("%s : %.1f"), *Pair.Key.ToString(), Pair.Value)));
				}
				else
				{
					TSharedPtr<STextBlock> Text = SNew(STextBlock).Text(FText::FromString(FString::Printf(TEXT("%s : %.1f"), *Pair.Key.ToString(), Pair.Value)));
					AbilityTexts.Add(Pair.Key, Text);
					if (AbilityBox.IsValid())
					{
						AbilityBox->AddSlot().AutoHeight()[ Text.ToSharedRef() ];
					}
				}
			}
		}
	}
	return EActiveTimerReturnType::Continue;
}


