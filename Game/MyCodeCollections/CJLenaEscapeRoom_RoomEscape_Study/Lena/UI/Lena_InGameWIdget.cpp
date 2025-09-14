// Fill out your copyright notice in the Description page of Project Settings.


#include "Lena_InGameWIdget.h"

#include "Lena_InGameGuideWidget.h"
#include "TimerManager.h"
#include "Components/CanvasPanelSlot.h"
#include "Engine/GameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Lena/GameInstanceSubSystem/LenaEnemyManager.h"
#include "Lena/GameInstanceSubSystem/LenaRoom01Manager.h"
#include "Lena/GameInstanceSubSystem/LenaUIManager.h"

bool ULena_InGameWIdget::Initialize()
{
	bool bSuccess = Super::Initialize();
	if(bSuccess == false) return false;
	return true;
}

void ULena_InGameWIdget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	GetAmmoInGun();
	GetAmmoInInventory();
	GetHealth();
	GetEnemyInMap();
}

void ULena_InGameWIdget::LoadData(TArray<FLenaUIGuide> _lenaUIGuide)
{
	if(_lenaUIGuide.IsEmpty() == false)
		LenaUIGuideData = _lenaUIGuide;
}

void ULena_InGameWIdget::HiddenInit()
{
	HiddenDescriptionText();
	if(g_LenaEnemyManagerIsValid)
	{
		int32 leftenemy = g_LenaEnemyManager->GetEnemyCount();
		FString leftenemystring = FString::FromInt(leftenemy);
		TextBlock_EnemyAll->SetText(FText::FromString(leftenemystring));
	}
}

void ULena_InGameWIdget::SetHealthPercent(float _input)
{
	if(Progressbar_Healthbar)
		Progressbar_Healthbar->SetPercent(_input);
}

void ULena_InGameWIdget::SetDescriptionText(FString _string)
{
	if(Lena_TimeRemainingWidget && Lena_TimeRemainingWidget->TextBlock_Description)
		Lena_TimeRemainingWidget->SetDescriptionText(_string);
}

void ULena_InGameWIdget::VisibleDescriptionText()
{
	if(Lena_TimeRemainingWidget)
		Lena_TimeRemainingWidget->SetVisibility(ESlateVisibility::Visible);
}

void ULena_InGameWIdget::HiddenDescriptionText()
{
	if(Lena_TimeRemainingWidget)
		Lena_TimeRemainingWidget->SetVisibility(ESlateVisibility::Hidden);
}

void ULena_InGameWIdget::VisibleDescriptionTextRoom03Win()
{
	if(Lena_TimeRemainingWidget)
	{
		Lena_TimeRemainingWidget->SetVisibility(ESlateVisibility::Visible);
	}
}

void ULena_InGameWIdget::UpdateDescriptionTextRoom03Win(int32 _time)
{
	if(Lena_TimeRemainingWidget)
	{
		FString timestring = FString::FromInt(_time);
		Lena_TimeRemainingWidget->SetDescriptionText(timestring);
	}
}

void ULena_InGameWIdget::OpenIntroScene()
{
	UGameplayStatics::OpenLevel(this, TEXT("IntroScene"), false);
}

void ULena_InGameWIdget::CreateGuideWidget()
{
	if(g_LenaUIManaerIsValid)
	{
		g_LenaUIManager->CreateGuideWidget(true);
	}
}

void ULena_InGameWIdget::HideEnemyLeftWidget()
{
	if(HorizontalBox_EnemyLeft)
		HorizontalBox_EnemyLeft->SetVisibility(ESlateVisibility::Hidden);
	if(SizeBox_EnemyLeft)
		SizeBox_EnemyLeft->SetVisibility(ESlateVisibility::Hidden);	
}

void ULena_InGameWIdget::GetAmmoInInventory()
{
	if(g_LenaUIManaerIsValid)
	{
		if(ABase_Character* character = g_LenaUIManager->GetBaseCharacter())
		{
			if(AGun* _gun = Cast<AGun>(character->CurrentWeapon))
			{
				int32 index = character->GetAmmoIndex(_gun->AmmoType);
				if(index != INDEX_NONE)
				{
					FString _ammo = FString::FromInt(character->InventoryComponent->Items[index].Quantity);
					TextBlock_AmmoAll->SetText(FText::FromString(_ammo));
				}
				else
				{
					TextBlock_AmmoAll->SetText(FText::FromString(""));
				}
			}
			else
			{
				TextBlock_AmmoAll->SetText(FText::FromString(""));
			}
		}
	}
}

void ULena_InGameWIdget::GetAmmoInGun()
{
	if(g_LenaUIManaerIsValid)
	{
		if(ABase_Character* character = g_LenaUIManager->GetBaseCharacter())
		{
			if(AGun* _gun = Cast<AGun>(character->CurrentWeapon))
			{
				FString _ammo = FString::FromInt(_gun->GetAmmo());
				TextBlock_AmmoLeft->SetText(FText::FromString(_ammo));
			}
			else
			{
				TextBlock_AmmoLeft->SetText(FText::FromString(""));
			}
		}
		else
		{
			TextBlock_AmmoLeft->SetText(FText::FromString(""));
		}
	}
}

void ULena_InGameWIdget::GetEnemyInMap()
{
	if(g_LenaEnemyManagerIsValid)
	{
		int32 leftenemy = g_LenaEnemyManager->GetEnemyCount();
		FString leftenemystring = FString::FromInt(leftenemy);
		TextBlock_EnemyLeft->SetText(FText::FromString(leftenemystring));
	}
}

void ULena_InGameWIdget::GetHealth()
{

	if(g_LenaUIManaerIsValid)
	{
		if(ABase_Character* character = g_LenaUIManager->GetBaseCharacter())
		{
			Progressbar_Healthbar->SetPercent(character->GetHealthPercent());
			FString _string = FString::Printf(TEXT("%.0f"), character->GetHealth());
			TextBlock_Health->SetText(FText::FromString(_string));
		}
		else
		{
			Progressbar_Healthbar->SetPercent(0);
			TextBlock_Health->SetText(FText::FromString(""));
		}
	}
	
}
