// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/NameInputWidget.h"

#include "Blueprint/WidgetTree.h"
#include "Components/Border.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Components/EditableTextBox.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/VerticalBox.h"


void UNameInputWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

TSharedRef<SWidget> UNameInputWidget::RebuildWidget()
{
	// Canvas Panel 생성
	UCanvasPanel* RootPanel = WidgetTree->ConstructWidget<UCanvasPanel>(UCanvasPanel::StaticClass(), TEXT("RootCanvas"));
	WidgetTree->RootWidget = RootPanel;

	// DPI 스케일을 1.0으로 설정
	SetDesiredSizeInViewport(FVector2D(1920.f, 1080.f));
	SetRenderTransformPivot(FVector2D(0.5f, 0.5f));

	// Vertical Box 생성 (500x500 크기)
	UVerticalBox* VerticalBox = WidgetTree->ConstructWidget<UVerticalBox>(UVerticalBox::StaticClass(), TEXT("VerticalBox"));

	// VerticalBox를 RootPanel에 추가하고 중앙에 배치
	UCanvasPanelSlot* VerticalBoxSlot = RootPanel->AddChildToCanvas(VerticalBox);
	VerticalBoxSlot->SetAnchors(FAnchors(0.5f, 0.5f));
	VerticalBoxSlot->SetAlignment(FVector2D(0.5f, 0.5f));
	VerticalBoxSlot->SetSize(FVector2D(500.f, 500.f));
	VerticalBoxSlot->SetPosition(FVector2D(0.f, 0.f));

	// 각 Border를 VerticalBox에 추가
	VerticalBox->AddChildToVerticalBox(CreateTitleSlot(FText::FromString("Put in your Name")));
	VerticalBox->AddChildToVerticalBox(CreateTextInputBoxSlot(FText::FromString("New NickName")));
	VerticalBox->AddChildToVerticalBox(CreateButtonSlot());

	return Super::RebuildWidget();
}

UBorder* UNameInputWidget::CreateTitleSlot(FText Title)
{
	// 첫 번째 Border 생성 (텍스트 박스 포함)
	UBorder* Border = WidgetTree->ConstructWidget<UBorder>(UBorder::StaticClass(), TEXT("FirstBorder"));
	Border->SetBrushColor(FLinearColor::Gray);
	Border->SetPadding(FMargin(0,0,0,0));
	Border->SetHorizontalAlignment(HAlign_Center);

	// 텍스트 블록 생성
	UTextBlock* Text = WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass(), TEXT("FirstText"));
	Text->SetText(Title);
	FSlateFontInfo FontInfo = FCoreStyle::GetDefaultFontStyle("Regular", 36);
	Text->SetFont(FontInfo); // 폰트 적용

	// FirstTextBox를 FirstBorder에 추가
	Border->SetContent(Text);

	return Border;
}

UBorder* UNameInputWidget::CreateTextInputBoxSlot(FText HintText)
{
	// Second Border 생성 (텍스트 박스 포함)
	UBorder* Border = WidgetTree->ConstructWidget<UBorder>(UBorder::StaticClass(), TEXT("SecondBorder"));
	Border->SetBrushColor(FLinearColor::Gray);
	Border->SetPadding(FMargin(0,150,0,150));

	// 텍스트 입력 박스 생성 (200x100 크기)
	NameTextBox = WidgetTree->ConstructWidget<UEditableTextBox>(UEditableTextBox::StaticClass(), TEXT("SecondTextBox"));
	NameTextBox->SetMinDesiredWidth(200.f);
	FSlateFontInfo FontInfo = FCoreStyle::GetDefaultFontStyle("Regular", 36);
	NameTextBox->WidgetStyle.SetFont(FontInfo); // WidgetStyle에 폰트 적용
	NameTextBox->SetHintText(HintText);
	NameTextBox->SetJustification(ETextJustify::Center);

	// 텍스트 박스 배경을 투명하게 설정
	FSlateBrush TransparentBrush;
	TransparentBrush.TintColor = FSlateColor(FLinearColor(0, 0, 0, 0)); // 알파 값을 0으로 설정하여 투명하게 만듦
	NameTextBox->WidgetStyle.BackgroundImageNormal = TransparentBrush;
	NameTextBox->WidgetStyle.BackgroundImageHovered = TransparentBrush;
	NameTextBox->WidgetStyle.BackgroundImageFocused = TransparentBrush;

	// 텍스트 색상 설정 (하얀색)
	NameTextBox->WidgetStyle.ForegroundColor = FSlateColor(FLinearColor::White);
	NameTextBox->WidgetStyle.FocusedForegroundColor = FSlateColor(FLinearColor::White);

	// SecondTextBox를 SecondBorder에 추가
	Border->SetContent(NameTextBox);

	return Border;
}

UBorder* UNameInputWidget::CreateButtonSlot()
{
	// Third Border 생성 (버튼 포함)
	UBorder* Border = WidgetTree->ConstructWidget<UBorder>(UBorder::StaticClass(), TEXT("ThirdBorder"));
	Border->SetBrushColor(FLinearColor::Gray);
	Border->SetHorizontalAlignment(HAlign_Center);

	// 버튼 생성
	ConfirmButton = WidgetTree->ConstructWidget<UButton>(UButton::StaticClass(), TEXT("ConfirmButton"));
	UTextBlock* ButtonText = WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass(), TEXT("ButtonText"));
	ButtonText->SetText(FText::FromString(TEXT("Confirm")));
	ConfirmButton->AddChild(ButtonText);

	// 버튼을 ThirdBorder에 추가
	Border->SetContent(ConfirmButton);

	return Border;
}
