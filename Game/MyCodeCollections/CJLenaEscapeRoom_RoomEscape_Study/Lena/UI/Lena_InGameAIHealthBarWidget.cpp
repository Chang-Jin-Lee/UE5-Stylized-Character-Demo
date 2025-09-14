// Fill out your copyright notice in the Description page of Project Settings.


#include "Lena_InGameAIHealthBarWidget.h"

#include "Engine/GameInstance.h"
#include "Lena/Characters/AI_Character.h"
#include "Lena/GameInstanceSubSystem/LenaUIManager.h"


bool ULena_InGameAIHealthBarWidget::Initialize()
{
	return Super::Initialize();
}

void ULena_InGameAIHealthBarWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}

void ULena_InGameAIHealthBarWidget::HiddenInit()
{
	
}

void ULena_InGameAIHealthBarWidget::SetHealthPercent(float _input)
{
	if(Progressbar_Healthbar)
		Progressbar_Healthbar->SetPercent(_input);
}

void ULena_InGameAIHealthBarWidget::GetHealth(TObjectPtr<AAI_Character> character)
{
	if(character)
	{
		Progressbar_Healthbar->SetPercent(character->GetHealthPercent());
		FString _string = FString::Printf(TEXT("%.0f"), character->GetHealth());
		TextBlock_Health->SetText(FText::FromString(_string));
	}
	else
	{
		if(Progressbar_Healthbar)
			Progressbar_Healthbar->SetPercent(0);
		if(TextBlock_Health)
			TextBlock_Health->SetText(FText::FromString(""));
	}
}

void ULena_InGameAIHealthBarWidget::GetAmmoInInventory(TObjectPtr<AAI_Character> character)
{
	if(character && character->CurrentWeapon)
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
	else
	{
		TextBlock_AmmoAll->SetText(FText::FromString(""));
	}
}

void ULena_InGameAIHealthBarWidget::GetAmmoInGun(TObjectPtr<AAI_Character> character)
{
	if(character)
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