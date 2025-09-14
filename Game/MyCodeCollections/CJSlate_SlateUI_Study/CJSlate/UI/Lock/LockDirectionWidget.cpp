// Fill out your copyright notice in the Description page of Project Settings.


#include "LockDirectionWidget.h"

#include "CJSlate/CJSlatePlayerController.h"
#include "CJSlate/Item/LockActor.h"
#include "Components/Button.h"
#include "Net/UnrealNetwork.h"

void ULockDirectionWidget::NativeConstruct()
{
	Super::NativeConstruct();

	Button_Direction->OnPressed.AddDynamic(this, &ULockDirectionWidget::RotateLockButton);
}

void ULockDirectionWidget::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ULockDirectionWidget, m_ATargetLockActor);
	DOREPLIFETIME(ULockDirectionWidget, m_iRotationButtonIndex);
}

void ULockDirectionWidget::LoadData(ALockActor* LockActor, int32 ButtonIndex)
{
	if (LockActor)
		m_ATargetLockActor = LockActor;
	if (ButtonIndex != -1)
		m_iRotationButtonIndex = ButtonIndex;
}

void ULockDirectionWidget::RotateLockButton()
{
	ACJSlatePlayerController* pc = Cast<ACJSlatePlayerController>(GetOwningPlayer());
	if (pc == nullptr) return;
	if (m_ATargetLockActor)
	{
		const FGameplayTag DirectionalTag = FGameplayTag::RequestGameplayTag(FName("Item.Lock.Directional"));
		const FGameplayTag CombinationAlphabetTag = FGameplayTag::RequestGameplayTag(FName("Item.Lock.Combination.Alphabet"));
		const FGameplayTag CombinationNumberTag = FGameplayTag::RequestGameplayTag(FName("Item.Lock.Combination.Number"));
		const FGameplayTag ButtonTag = FGameplayTag::RequestGameplayTag(FName("Item.Lock.Button"));
		const FGameplayTagContainer& ItemTags = m_ATargetLockActor->m_fitemInfo.ItemTag;
		if (ItemTags.HasTag(DirectionalTag))
		{
			switch (m_iRotationButtonIndex)
			{
			case 0:
				pc->Server_LockMoveFromStart(m_ATargetLockActor, FVector(0,0,1), m_iRotationButtonIndex);
				// m_ATargetLockActor->MoveFromStart(FVector(0,0,1), m_iRotationButtonIndex);
				break;
			case 1:
				pc->Server_LockMoveFromStart(m_ATargetLockActor, FVector(1,0,0), m_iRotationButtonIndex);
				// m_ATargetLockActor->MoveFromStart(FVector(1,0,0), m_iRotationButtonIndex);
				break;
			case 2:
				pc->Server_LockMoveFromStart(m_ATargetLockActor, FVector(0,0,-1), m_iRotationButtonIndex);
				// m_ATargetLockActor->MoveFromStart(FVector(0,0,-1), m_iRotationButtonIndex);
				break;
			case 3:
				pc->Server_LockMoveFromStart(m_ATargetLockActor, FVector(-1,0,0), m_iRotationButtonIndex);
				// m_ATargetLockActor->MoveFromStart(FVector(-1,0,0), m_iRotationButtonIndex);
				break;
			default:
				break;
			}
		}
		else if (ItemTags.HasTag(CombinationNumberTag))
		{
			pc->Server_LockMoveFromStart(m_ATargetLockActor, FVector(-1,-1,-1), m_iRotationButtonIndex);
			// m_ATargetLockActor->MoveFromStart(FVector(-1,-1,-1), m_iRotationButtonIndex);
		}
		else if (ItemTags.HasTag(ButtonTag))
		{
			pc->Server_LockMoveFromStart(m_ATargetLockActor, FVector(-1,-1,-1), m_iRotationButtonIndex);
			// m_ATargetLockActor->MoveFromStart(FVector(-1,-1,-1), m_iRotationButtonIndex);
		}
		// m_ATargetLockActor->MoveFromStart(FVector(0, 0, 0), nullptr, m_iRotationButtonIndex);
	}
}
