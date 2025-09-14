// Fill out your copyright notice in the Description page of Project Settings.


#include "SCJSlateOptionMenuWidget.h"

#include "LobbyPanelWidget.h"
#include "Blueprint/UserWidget.h"
#include "CJSlate/HUD/CJSlateHUD.h"
#include "CJSlate/CJSlatePlayerController.h"
#include "CJSlate/GameInstance/CJSlateGameInstance.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"

SCJSlateOptionMenuWidget::SCJSlateOptionMenuWidget()
{
}

SCJSlateOptionMenuWidget::~SCJSlateOptionMenuWidget()
{
}

#define LOCTEXT_NAMESPACE "SCJSlateOptionMenuWidget"

void SCJSlateOptionMenuWidget::Construct(const FArguments& args)
{
	OwningHUD = args._OwningHUD;
	
	const FMargin ContentPadding = FMargin(500.0f,300.0f);
	const FMargin ButtonPadding = FMargin(5.0f,5.0f);
	const FText TitleText = FText::FromString(TEXT("Option"));
	const FText PlayText = FText::FromString(TEXT("Back To Game"));
	const FText BackToTitleText = FText::FromString(TEXT("Back To Title"));
	const FText CJSlateOptionText = FText::FromString(TEXT("Setting"));
	const FText ExitText = FText::FromString(TEXT("Quit Game"));
	FSlateFontInfo TitleTextSyle = FCoreStyle::Get().GetFontStyle("EmbossedText");
	TitleTextSyle.Size = 40.0f;
	FSlateFontInfo ButtonTextSyle = TitleTextSyle;
	ButtonTextSyle.Size = 50.0f;
	
	ChildSlot
	[
		SNew(SOverlay)
		+ SOverlay::Slot()
		.HAlign(HAlign_Fill)
		.VAlign(VAlign_Fill)
		[
			SNew(SImage)
			.ColorAndOpacity(FColor(0,0,0, 125))
		]
		+ SOverlay::Slot()
		.HAlign(HAlign_Fill)
		.VAlign(VAlign_Fill)
		.Padding(ContentPadding)
		[
			SNew(SVerticalBox)
			
			+ SVerticalBox::Slot()
			[
				SNew(STextBlock)
				.Font(TitleTextSyle)
				.Text(TitleText)
				.Justification(ETextJustify::Center)
			]

			+ SVerticalBox::Slot()
			.Padding(ButtonPadding)
			[
				SNew(SButton)
				[
					SNew(STextBlock)
					.Font(ButtonTextSyle)
					.Text(PlayText)
					.Justification(ETextJustify::Center)
				]
				.OnClicked(this, &SCJSlateOptionMenuWidget::OnPlayButtonCliked)
			]

			+ SVerticalBox::Slot()
			.Padding(ButtonPadding)
			[
				SNew(SButton)
				[
					SNew(STextBlock)
					.Font(ButtonTextSyle)
					.Text(BackToTitleText)
					.Justification(ETextJustify::Center)
				]
				.OnClicked(this, &SCJSlateOptionMenuWidget::OnBackToTitleButtonCliked)
			]

			+ SVerticalBox::Slot()
			.Padding(ButtonPadding)
			[
				SNew(SButton)
				[
					SNew(STextBlock)
					.Font(ButtonTextSyle)
					.Text(CJSlateOptionText)
					.Justification(ETextJustify::Center)
				]
				.OnClicked(this, &SCJSlateOptionMenuWidget::OnCJSlateOptionButtonCliked)
			]

			+ SVerticalBox::Slot()
			.Padding(ButtonPadding)
			[
				SNew(SButton)
				[
					SNew(STextBlock)
					.Font(ButtonTextSyle)
					.Text(ExitText)
					.Justification(ETextJustify::Center)
				]
				.OnClicked(this, &SCJSlateOptionMenuWidget::OnExitButtonCliked)
			]
		]
	];
}

FReply SCJSlateOptionMenuWidget::OnPlayButtonCliked()
{
	if (OwningHUD.IsValid())
	{
		OwningHUD->ExitMenu();
	}
	
	return FReply::Handled();
}

FReply SCJSlateOptionMenuWidget::OnBackToTitleButtonCliked()
{
	// 잠깐 주석처리. 데디서버에게 세션 끊고 나가긴 해야함
	// if (UCJSlateGameInstance* gi = Cast<UCJSlateGameInstance>(OwningHUD->GetGameInstance()))
	// {
	// 	gi->EndSession();
	// 	gi->DestroySession();
	// }

	// 데디서버를 쓸때 쓸 것. 스팀에서는 삭제
	APlayerController* PC = UGameplayStatics::GetPlayerController(OwningHUD->GetWorld(),0);
	if (PC)
		PC->ClientTravel("Title", TRAVEL_Absolute);
	
	return FReply::Handled();
}

FReply SCJSlateOptionMenuWidget::OnCJSlateOptionButtonCliked()
{
	FStringClassReference MyWidgetClassRef(TEXT("/Game/UI/WBP_LobbyPannelWidget.WBP_LobbyPannelWidget_C"));
	
	if ( UClass* MyWidgetClass = MyWidgetClassRef.TryLoadClass<UUserWidget>() )
	{
		ULobbyPanelWidget* MyWidget = CreateWidget<ULobbyPanelWidget>(OwningHUD->GetWorld(), MyWidgetClass);
		MyWidget->ReturnButton->SetVisibility(ESlateVisibility::Visible);
		MyWidget->AddToViewport();
	}
	
	return FReply::Handled();
}

FReply SCJSlateOptionMenuWidget::OnExitButtonCliked()
{
	// 잠시 주석 처리. SessionName을 넣어야해서 나중에 넣기
	// if (UCJSlateGameInstance* gi = Cast<UCJSlateGameInstance>(OwningHUD->GetGameInstance()))
	// {
	// 	gi->EndSession();
	// }
	
	if (OwningHUD.IsValid())
	{
		//OwningHUD->ExitMenu();
		if (APlayerController* PC = OwningHUD->PlayerOwner)
		{
			PC->ConsoleCommand("quit");
			//PC->ExitMenu();
		}
	}
	return FReply::Handled();
}

#undef LOCTEXT_NAMESPACE