// Fill out your copyright notice in the Description page of Project Settings.


#include "OptionMenu.h"

#include "Blueprint/WidgetTree.h"
#include "Components/Button.h"
#include "Components/ButtonSlot.h"
#include "Components/HorizontalBox.h"
#include "Components/SizeBox.h"
#include "Components/SizeBoxSlot.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"
#include "Components/VerticalBoxSlot.h"

TSharedRef<SWidget> UOptionMenu::RebuildWidget()
{
	SetDesiredSizeInViewport(FVector2d(300,240));
	SetRenderTransformPivot(FVector2D(0,0));
	
	USizeBox* SizeBox = WidgetTree->ConstructWidget<USizeBox>(USizeBox::StaticClass(), TEXT("OptionMenuSizeBox"));
	SizeBox->SetWidthOverride(300);
	SizeBox->SetHeightOverride(240);
	WidgetTree->RootWidget = SizeBox;
	
	UVerticalBox* VerticalBox1 = AddVerticalBox(SizeBox, FName("OptionMenuVerticalBox1"));
	AddButton(VerticalBox1, FName("OptionMenuButton1"), FName("삭제하기"), 28);
	AddButton(VerticalBox1, FName("OptionMenuButton2"), FName(""), 28);
	AddButton(VerticalBox1, FName("OptionMenuButton3"), FName(""), 28);
	
	return Super::RebuildWidget();
}

template <typename T>
UVerticalBox* UOptionMenu::AddVerticalBox(T* ParentWidget, FName VerticalBoxName)
{
	UVerticalBox* VerticalBox = WidgetTree->ConstructWidget<UVerticalBox>(UVerticalBox::StaticClass(), VerticalBoxName);

	if(USizeBox* SizeBox = Cast<USizeBox>(ParentWidget))
	{
		SizeBox->AddChild(VerticalBox);
		USizeBoxSlot* SizeBoxSlot = Cast<USizeBoxSlot>(SizeBox->GetContentSlot());
		if(SizeBoxSlot)
		{
			SizeBoxSlot->SetHorizontalAlignment(HAlign_Fill);
			SizeBoxSlot->SetVerticalAlignment(VAlign_Fill);
			SizeBoxSlot->SetPadding(FMargin(0,0,0,0));
		}
	}

	return VerticalBox;
}

template <typename T>
UButton* UOptionMenu::AddButton(T* ParentWidget, FName ButtonBlockName, FName ButtonName, float FontSize)
{
	UButton* Button = WidgetTree->ConstructWidget<UButton>(UButton::StaticClass(), ButtonBlockName);

	if(UVerticalBox* VerticalBox = Cast<UVerticalBox>(ParentWidget))
	{
		UVerticalBoxSlot* VerticalBoxSlot = VerticalBox->AddChildToVerticalBox(Button);
		
		FSlateChildSize ChildSize;
		ChildSize.Value = 1.0f;
		VerticalBoxSlot->SetSize(ChildSize);
		VerticalBoxSlot->SetHorizontalAlignment(HAlign_Fill);
		VerticalBoxSlot->SetVerticalAlignment(VAlign_Fill);
		VerticalBoxSlot->SetPadding(FMargin(0,5,0,0));
	}

	FString ButtonBlockNameWithText = ButtonBlockName.ToString() + TEXT("text");
	FName ButtonTextBlockName = FName(*ButtonBlockNameWithText);
	
	UTextBlock* TextBlock = WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass(), ButtonTextBlockName);
	if(ButtonName == FName("삭제하기"))
	{
		TextBlock->SetText(FText::FromName(TEXT("삭제하기")));
		Button->OnClicked.AddDynamic(this, &UOptionMenu::OnButtonClicked);
	}

	FSlateFontInfo FontInfo = FCoreStyle::GetDefaultFontStyle("Regular", FontSize);
	TextBlock->SetFont(FontInfo);

	if(Button)
	{
		Button->AddChild(TextBlock);
		UButtonSlot* ButtonSlot = Cast<UButtonSlot>(Button->GetContentSlot());
		
		if(ButtonSlot)
		{
			ButtonSlot->SetHorizontalAlignment(EHorizontalAlignment::HAlign_Center);
			ButtonSlot->SetVerticalAlignment(VAlign_Center);
		}
	}
	
	return Button;
}

void UOptionMenu::OnButtonClicked()
{
	if(ensure(TargetUnit))
	{
		TargetUnit->Destroy();
		RemoveFromParent();
	}
}