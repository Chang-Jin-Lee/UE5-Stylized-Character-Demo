// Fill out your copyright notice in the Description page of Project Settings.


#include "Lena_InGameGuideWidget.h"

#include "Components/Button.h"
#include "Lena/GameInstanceSubSystem/LenaUIManager.h"

bool ULena_InGameGuideWidget::Initialize()
{
	bool bSuccess = Super::Initialize();
	if(bSuccess == false) return false;
	
	if(Button_Prev)
	{
		Button_Prev->OnClicked.AddDynamic(this, &ULena_InGameGuideWidget::OnButtonPrev);
	}
	if(Button_Next)
	{
		Button_Next->OnClicked.AddDynamic(this, &ULena_InGameGuideWidget::OnButtonNext);
	}
	if(Button_Exit)
	{
		Button_Exit->OnClicked.AddDynamic(this, &ULena_InGameGuideWidget::OnButtonExit);
	}
	return true;
}

void ULena_InGameGuideWidget::LoadData()
{
	if(g_LenaUIManaerIsValid)
	{
		UIGuidesData = g_LenaUIManager->GetLenaLenaUIGuide();
		FString _text = UIGuidesData[currentIndex].Text;
		_text.ReplaceInline(TEXT("\\n"), TEXT("\n"));
		TextBlock_Description->SetText(FText::FromString(_text));
		UTexture2D* _texture = LoadObject<UTexture2D>(nullptr, *(UIGuidesData[currentIndex].Image));
		if(_texture)
		{
			Image_Description->SetBrushFromTexture(_texture);
		}
	}
}

void ULena_InGameGuideWidget::OnButtonPrev()
{
	
	if(UIGuidesData.Num() > 0)
	{
		currentIndex = FMath::Max(0, currentIndex-1);
		FString _text = UIGuidesData[currentIndex].Text;
		_text.ReplaceInline(TEXT("\\n"), TEXT("\n"));
		TextBlock_Description->SetText(FText::FromString(_text));
		UTexture2D* _texture = LoadObject<UTexture2D>(nullptr, *(UIGuidesData[currentIndex].Image));
		if(_texture)
		{
			Image_Description->SetBrushFromTexture(_texture);
		}
	}
}

void ULena_InGameGuideWidget::OnButtonNext()
{
	if(UIGuidesData.Num() > 0)
	{
		if(currentIndex == UIGuidesData.Num()-1)
		{
			OnButtonExit();
		}
		currentIndex = FMath::Min(UIGuidesData.Num()-1, currentIndex+1);
		FString _text = UIGuidesData[currentIndex].Text;
		_text.ReplaceInline(TEXT("\\n"), TEXT("\n"));
		TextBlock_Description->SetText(FText::FromString(_text));
		UTexture2D* _texture = LoadObject<UTexture2D>(nullptr, *(UIGuidesData[currentIndex].Image));
		if(_texture)
		{
			Image_Description->SetBrushFromTexture(_texture);
		}
	}
}

void ULena_InGameGuideWidget::OnButtonExit()
{
	if(g_LenaUIManaerIsValid)
	{
		if(bIsStartGuide)
			g_LenaUIManager->GetShooterPlayerController()->InputModeGame();
	}
	RemoveFromParent();
}
