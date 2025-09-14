// Fill out your copyright notice in the Description page of Project Settings.


#include "ButtonActor.h"

#include "CJSlate/CJSlatePlayerController.h"
#include "CJSlate/Door/DoorActor.h"
#include "CJSlate/Managers/AnswerManager.h"
#include "CJSlate/Stone/StoneActor.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"


AButtonActor::AButtonActor()
{
	PrimaryActorTick.bCanEverTick = true;

	bReplicates = true;
	SetReplicateMovement(true);

	MoveData.EventTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("EventTimeline"));
}

void AButtonActor::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AButtonActor, m_aTargetActors);
}

void AButtonActor::BeginPlay()
{
	Super::BeginPlay();
	if (MoveData.EventCurve)
	{
		MoveData.EventTimelineCallback.BindUFunction(this, FName("HandleEventProgress"));
		MoveData.EventTimeline->AddInterpFloat(MoveData.EventCurve, MoveData.EventTimelineCallback);
	}
	MoveData.InitialLocation = StaticMeshComponent->GetRelativeLocation();
}

void AButtonActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AButtonActor::HandleEventProgress(float Value)
{
	FVector NewLocation = FMath::Lerp(MoveData.InitialLocation, MoveData.TargetLocation, Value);
	StaticMeshComponent->SetRelativeLocation(NewLocation);
}

// void AButtonActor::ClickButton()
// {
// 	MoveData.EventTimeline->PlayFromStart();
// 	// 여기에 사운드도 추가하면 됨.
// 	OnButtonClicked.Broadcast();
// }

void AButtonActor::ClickButton_Implementation()
{
	MoveData.EventTimeline->PlayFromStart();
	// 여기에 사운드도 추가하면 됨.
}

void AButtonActor::HandleClick(TArray<int32> IsHoldings)
{
	if (m_fitemInfo.ItemTag.HasTag(FGameplayTag::RequestGameplayTag(FName("Item.Button.DoorButton"))))
	{
		for (auto TargetActor : m_aTargetActors)
		{
			if (ADoorActor* Door = Cast<ADoorActor>(TargetActor))
			{
				Door->OpenDoor();
			}
		}
	}
	else if (m_fitemInfo.ItemTag.HasTag(FGameplayTag::RequestGameplayTag(FName("Item.Button.UpAndDownButton"))))
	{
		for (auto TargetActor : m_aTargetActors)
		{
			if (AStoneActor* Stone = Cast<AStoneActor>(TargetActor))
			{
				Stone->Move();
			}
		}
		if (AStoneActor* Stone = Cast<AStoneActor>(m_aTargetActors[0]))
		{
			Stone->AnswerManager->IsCorrectAnswer();
		}
	}
	else if (m_fitemInfo.ItemTag.HasTag(FGameplayTag::RequestGameplayTag(FName("Item.Button.ClearButton"))))
	{
		for (auto TargetActor : m_aTargetActors)
		{
			if (AAnswerManager* AnswerManager = Cast<AAnswerManager>(TargetActor))
			{
				AnswerManager->ClearAnswers();
			}
		}
	}
	else if (m_fitemInfo.ItemTag.HasTag(FGameplayTag::RequestGameplayTag(FName("Item.Button.ValidKeyButton"))))
	{
		TMap<int32, int32> currentItems;	// 플레이어의 인벤토리에 있는 아이템을 TMap에 수량으로 매핑
		for (int32 itemID : IsHoldings)
		{
			if (currentItems.Find(itemID) == nullptr)
				currentItems.Add(itemID, 1);
			else
				currentItems[itemID]++;
		}
		
		for (auto RequiredKeyItemID : RequiredKeyItemIDs)
		{
			if (currentItems.Find(RequiredKeyItemID.Key) == nullptr)
				return;
			if (currentItems[RequiredKeyItemID.Key] < RequiredKeyItemID.Value)
				return;
		}
		
		for (auto TargetActor : m_aTargetActors)
		{
			if (ADoorActor* Door = Cast<ADoorActor>(TargetActor))
			{
				Door->OpenDoor();
			}
		}
	}
}
