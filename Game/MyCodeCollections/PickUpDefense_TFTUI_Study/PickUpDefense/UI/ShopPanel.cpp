// Fill out your copyright notice in the Description page of Project Settings.


#include "ShopPanel.h"

#include "ShopPanelSlot.h"
#include "Blueprint/DragDropOperation.h"
#include "Blueprint/WidgetTree.h"
#include "Components/Border.h"
#include "Components/HorizontalBox.h"
#include "Components/HorizontalBoxSlot.h"
#include "Components/Image.h"
#include "Components/Overlay.h"
#include "Components/OverlaySlot.h"
#include "Components/SizeBox.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"
#include "CardData.h"

TSharedRef<SWidget> UShopPanel::RebuildWidget()
{
	SetDesiredSizeInViewport(FVector2d(1700,220));
	SetRenderTransformPivot(FVector2D(0,0));
	
	USizeBox* SizeBox = WidgetTree->ConstructWidget<USizeBox>(USizeBox::StaticClass(), TEXT("SizeBox"));
	SizeBox->SetWidthOverride(1700);
	SizeBox->SetHeightOverride(220);
	WidgetTree->RootWidget = SizeBox;
	
	UHorizontalBox* BaseHorizontalBox = WidgetTree->ConstructWidget<UHorizontalBox>(UHorizontalBox::StaticClass(), TEXT("BaseHorizontalBox"));
	SizeBox->AddChild(BaseHorizontalBox);

	AddSideBox(BaseHorizontalBox, FName(TEXT("LeftHorizontalBox")), 0.2);
	AddMiddleBox(BaseHorizontalBox, FName(TEXT("MiddleOverlay")));
	AddSideBox(BaseHorizontalBox, FName(TEXT("RightHorizontalBox")), 0.1);
	
	return Super::RebuildWidget();
}


template <typename T>
UShopPanelSlot* UShopPanel::AddCardToChild(T* ParentWidget, FName BorderName)
{
	UShopPanelSlot* PanelSlot = WidgetTree->ConstructWidget<UShopPanelSlot>(UShopPanelSlot::StaticClass(), BorderName);
	// UBorder* Border = WidgetTree->ConstructWidget<UBorder>(UBorder::StaticClass(), BorderName);
	// Border->SetBrushColor(FLinearColor(0.735351, 0.35115, 1, 0.4));
	
	if(ParentWidget)
	{
		UHorizontalBoxSlot* BorderSlot = ParentWidget->AddChildToHorizontalBox(PanelSlot);
		FSlateChildSize ChildSize;
		ChildSize.Value = 1.0f;
		BorderSlot->SetSize(ChildSize);
		BorderSlot->SetHorizontalAlignment(EHorizontalAlignment::HAlign_Fill);
		BorderSlot->SetVerticalAlignment(EVerticalAlignment::VAlign_Fill);
		BorderSlot->SetPadding(FMargin(1,0,1,0));
	}

	return PanelSlot;
}

template <typename T>
void UShopPanel::AddSideBox(T* ParentWidget, FName SlotName, float Size)
{
	UHorizontalBox* HorizontalBox = WidgetTree->ConstructWidget<UHorizontalBox>(UHorizontalBox::StaticClass(), SlotName);
	UHorizontalBoxSlot* HorizontalBoxSlot = ParentWidget->AddChildToHorizontalBox(HorizontalBox);
	FSlateChildSize HorizontalBoxSlotSize;
	HorizontalBoxSlotSize.Value = Size;
	HorizontalBoxSlot->SetSize(HorizontalBoxSlotSize);

	// FName에 문자열을 붙여 새로운 FName 생성
	FName LeftBorderName = FName(*(SlotName.ToString() + TEXT("_LeftBorder")));
	FName RightBorderName = FName(*(SlotName.ToString() + TEXT("_RightBorder")));
	
	// 왼쪽 보더
	UBorder* HorizontalBox_LeftBorder = WidgetTree->ConstructWidget<UBorder>(UBorder::StaticClass(), LeftBorderName);
	HorizontalBox_LeftBorder->SetBrushColor(FLinearColor::Gray);
	UHorizontalBoxSlot* HorizontalBox_LeftBorderSlot = HorizontalBox->AddChildToHorizontalBox(HorizontalBox_LeftBorder);
	FSlateChildSize HorizontalBox_LeftBorderSlotSize;
	HorizontalBox_LeftBorderSlotSize.Value = 0.5;
	HorizontalBox_LeftBorderSlot->SetSize(HorizontalBox_LeftBorderSlotSize);
	HorizontalBox_LeftBorderSlot->SetPadding(FMargin(5,150,5,5));
	// 오른쪽 보더
	UBorder* HorizontalBox_RightBorder = WidgetTree->ConstructWidget<UBorder>(UBorder::StaticClass(), RightBorderName);
	HorizontalBox_RightBorder->SetBrushColor(FLinearColor::Gray);
	UHorizontalBoxSlot* HorizontalBox_RightBorderSlot = HorizontalBox->AddChildToHorizontalBox(HorizontalBox_RightBorder);
	FSlateChildSize HorizontalBox_LeftBorderSize;
	HorizontalBox_LeftBorderSize.Value = 1.0;
	HorizontalBox_RightBorderSlot->SetSize(HorizontalBox_LeftBorderSize);
	HorizontalBox_RightBorderSlot->SetPadding(FMargin(5,150,5,5));
}

template <typename T>
void UShopPanel::AddMiddleBox(T* ParentWidget, FName SlotName)
{
	UOverlay* MiddleOverlay = WidgetTree->ConstructWidget<UOverlay>(UOverlay::StaticClass(), SlotName);
	UHorizontalBoxSlot* MiddleOverlaySlot = ParentWidget->AddChildToHorizontalBox(MiddleOverlay);
	FSlateChildSize MiddleOverlaySlotSize;
	MiddleOverlaySlotSize.Value = 1.0;
	MiddleOverlaySlot->SetSize(MiddleOverlaySlotSize);
	MiddleOverlaySlot->SetHorizontalAlignment(EHorizontalAlignment::HAlign_Fill);
	MiddleOverlaySlot->SetVerticalAlignment(EVerticalAlignment::VAlign_Fill);
	UHorizontalBox* MiddleHorizontalBox = WidgetTree->ConstructWidget<UHorizontalBox>(UHorizontalBox::StaticClass(), TEXT("MiddleHorizontalBox"));
	UVerticalBox* MiddleVerticalBox = WidgetTree->ConstructWidget<UVerticalBox>(UVerticalBox::StaticClass(), TEXT("MiddleVerticalBox"));

	UOverlaySlot* MiddleHorizontalBoxSlot = MiddleOverlay->AddChildToOverlay(MiddleHorizontalBox);
	MiddleHorizontalBoxSlot->SetHorizontalAlignment(EHorizontalAlignment::HAlign_Fill);
	MiddleHorizontalBoxSlot->SetVerticalAlignment(EVerticalAlignment::VAlign_Fill);
	
	UOverlaySlot* MiddleVerticalBoxSlot = MiddleOverlay->AddChildToOverlay(MiddleVerticalBox);
	MiddleVerticalBoxSlot->SetPadding(FMargin(0,125,0,0));
	MiddleVerticalBoxSlot->SetHorizontalAlignment(EHorizontalAlignment::HAlign_Fill);
	MiddleVerticalBoxSlot->SetVerticalAlignment(EVerticalAlignment::VAlign_Fill);

	// 보더 추가
	// for(int i = 0; i < 5; i ++)
	// {
	// 	AddCardToChild<UHorizontalBox>(MiddleHorizontalBox, FName(TEXT("MiddleExampleBorder%d"), i));
	// }

	// 카드 데이터 설정
	// TArray<FCardData> CardList = {
	// 	{ TEXT("/Script/Engine.Texture2D'/Game/UI/Image/TFT12_Morgana.TFT12_Morgana'"), TEXT("/Script/Engine.StaticMesh'/Game/UI/TEST/Morgana.Morgana'"), { TEXT("마녀"), TEXT("박쥐여왕"), TEXT("보호술사") }, TEXT("모르가나"), TEXT("$5") },
	// 	{ TEXT("/Script/Engine.Texture2D'/Game/UI/Image/TFT11_Irelia.TFT11_Irelia'"), TEXT("/Script/Engine.StaticMesh'/Game/UI/TEST/Irelia.Irelia'"), { TEXT(""), TEXT("이야기꾼"), TEXT("결투가") }, TEXT("이렐리아"), TEXT("$5") },
	// 	{ TEXT("/Script/Engine.Texture2D'/Game/UI/Image/TFT11_Ahri.TFT11_Ahri'"), TEXT("/Script/Engine.StaticMesh'/Game/UI/TEST/Ahri.Ahri'"), { TEXT(""), TEXT("필연"), TEXT("비전마법사") }, TEXT("아리"), TEXT("$1") },
	// 	{ TEXT("/Script/Engine.Texture2D'/Game/UI/Image/TFT12_Rumble.TFT12_Rumble'"), TEXT("/Script/Engine.StaticMesh'/Game/UI/TEST/Rumble.Rumble'"), { TEXT("달콤술사"), TEXT("폭파단"), TEXT("선봉대") }, TEXT("럼블"), TEXT("$2") },
	// 	{ TEXT("/Script/Engine.Texture2D'/Game/UI/Image/TFT12_Fiora.TFT12_Fiora'"), TEXT("/Script/Engine.StaticMesh'/Game/UI/TEST/Fiora.Fiora'"), { TEXT(""), TEXT("마녀"), TEXT("전사") }, TEXT("피오라"), TEXT("$4") }
	// };

	
	// 먼저 데이터 테이블을 로드합니다.
	UDataTable* CardDataTable = LoadObject<UDataTable>(nullptr, TEXT("/Script/Engine.DataTable'/Game/DataTable/Cards/CardData.CardData'"));
	if (!ensure(CardDataTable)) return;

	// 카드 리스트를 데이터 테이블에서 가져옵니다.
	static const FString ContextString(TEXT("Card Context"));

	TArray<FCardData*> CardList;
	CardDataTable->GetAllRows(ContextString, CardList);

	// 패널 초기화: 데이터 테이블에서 가져온 카드 리스트를 기반으로 패널에 추가합니다.
	for (int32 i = 0; i < CardList.Num(); i++)
	{
		// MiddleHorizontalBox는 패널을 추가할 부모 위젯입니다.
		UShopPanelSlot* Panel = AddCardToChild<UHorizontalBox>(MiddleHorizontalBox, FName(TEXT("MiddleExampleBorder%d"), i + 1));

		if (Panel && CardList[i])
		{
			// InitializeCard 함수를 사용하여 패널에 카드 정보를 설정합니다.
			InitializeCard(Panel, *CardList[i]);  // CardList[i]는 포인터이므로 역참조 필요
		}
	}

}

void UShopPanel::InitializeCard(UShopPanelSlot* Panel, const FCardData& CardData)
{
	if (!Panel) return;
	
	Panel->CardName = CardData.Name;
	Panel->Synergy1Text = CardData.Synergies[0];
	Panel->Synergy2Text = CardData.Synergies[1];
	Panel->Synergy3Text = CardData.Synergies[2];
	Panel->PriceText = CardData.Price;
	Panel->ImageDir = CardData.TexturePath;
	Panel->MeshDir = CardData.MeshPath;

	if(CardData.Price == TEXT("$1"))
	{
		Panel->BorderColor = FLinearColor::FromSRGBColor(FColor::FromHex(TEXT("192833")));
	}
	else if(CardData.Price == TEXT("$2"))
	{
		Panel->BorderColor = FLinearColor::FromSRGBColor(FColor::FromHex(TEXT("12362d")));
	}
	else if(CardData.Price == TEXT("$3"))
	{
		Panel->BorderColor = FLinearColor::FromSRGBColor(FColor::FromHex(TEXT("1e2e58")));
	}
	else if(CardData.Price == TEXT("$4"))
	{
		Panel->BorderColor = FLinearColor::FromSRGBColor(FColor::FromHex(TEXT("540b4b")));
	}
	else if(CardData.Price == TEXT("$5"))
	{
		Panel->BorderColor = FLinearColor::FromSRGBColor(FColor::FromHex(TEXT("7d4b02")));
	}
	
		
	Panel->UpdateCardInfo();
}

