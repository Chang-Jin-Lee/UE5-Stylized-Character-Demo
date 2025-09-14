// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractManager.h"
#include "EngineUtils.h"
#include "Lena/Actor/InteractableThings/Door/DoorActor.h"
#include "Lena/Actor/InteractableThings/Lock/LockActor.h"
#include "Engine/World.h"
#include "Lena/Actor/InteractableThings/Button/ButtonActor.h"
#include "Lena/Interface/InteractActionInterface.h"
#include "Lena/Actor/Items/Base_Item.h"
#include "Kismet/GameplayStatics.h"
#include "Lena/GameInstance/LenaGameInstance.h"
#include "Lena/GameInstanceSubSystem/LenaRoom01Manager.h"
#include "Lena/GameInstanceSubSystem/LenaRoom02Manager.h"
#include "Lena/GameInstanceSubSystem/LenaRoom03Manager.h"

AInteractManager::AInteractManager()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AInteractManager::BeginPlay()
{
	Super::BeginPlay();

	GetWorld()->GetTimerManager().SetTimerForNextTick(this, &AInteractManager::SetupLockAndDoor);
}

// Called every frame
void AInteractManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AInteractManager::SetupLockAndDoor()
{
	// 액터 매핑 초기화
	// for(TActorIterator<AActor> It(GetWorld()); It; ++It) // #include "EngineUtils.h" 필요
	for(AActor* It : InteractActors)
	{
		if(ALockActor* _lockactor = Cast<ALockActor>(It))
		{
			LockMap.Add(_lockactor->GetName(), _lockactor);
		}
		else if (ADoorActor* _dooractor = Cast<ADoorActor>(It))
		{
			DoorMap.Add(_dooractor->GetName(), _dooractor);
		}
		else if (ABase_Item* _baseitemactor = Cast<ABase_Item>(It))
		{
			if(ItemMap.Contains(_baseitemactor->ItemName))
			{
				ItemMap[_baseitemactor->ItemName].Add(_baseitemactor);
			}
			else
			{
				TArray<AActor*> NewArray;
				NewArray.Add(_baseitemactor);
				ItemMap.Add(_baseitemactor->ItemName, TArray<TObjectPtr<AActor>>(NewArray));
			}
		}
		else if (TObjectPtr<AButtonActor> _buttonactor = Cast<AButtonActor>(It))
		{
			ButtonMap.Add(_buttonactor->GetName(), _buttonactor);
		}
	}
	// Door 기준

	FString MapName = GetWorld()->GetMapName();
	FString CleanMapName = MapName.Mid(GetWorld()->StreamingLevelsPrefix.Len());

	if(CleanMapName == "Room01_Lab")
	{
		if(g_LenaRoom01ManagerIsValid)
		{
			for(FLenaRoom01InteractConditionRow Row : g_LenaRoom01Manager->GetLenaRoom01InteractConditionData())
			{
				TObjectPtr<AActor>* FoundActor = DoorMap.Find(Row.ActorID);
				if(FoundActor)
				{
					FConditionEntry Condition;
					Condition.ConditionID = Row.ConditionID;
					Condition.ConditionType = Row.ConditionType;
					Condition.AdditionalData = Row.AdditionalData;
					SetupConditionWithActor(*FoundActor, Condition);
				}
			}
		}
	}
	else if (CleanMapName == "Room02_Outside")
	{
		if(g_LenaRoom02ManagerIsValid)
		{
			for(FLenaRoom02InteractConditionRow Row : g_LenaRoom02Manager->GetLenaRoom02InteractConditionData())
			{
				TObjectPtr<AActor>* FoundActor = DoorMap.Find(Row.ActorID);
				if(FoundActor)
				{
					FConditionEntry Condition;
					Condition.ConditionID = Row.ConditionID;
					Condition.ConditionType = Row.ConditionType;
					Condition.AdditionalData = Row.AdditionalData;
					SetupConditionWithActor(*FoundActor, Condition);
				}
			}
		}
	}
}

void AInteractManager::SetupConditionWithActor(TObjectPtr<AActor> Actor, const FConditionEntry& ConditionEntry)
{
	UE_LOG(LogTemp, Warning, TEXT("SetupConditionWithActor start"));
	if(ConditionEntry.ConditionType == "Lock")
	{
		ADoorActor* DoorActor = Cast<ADoorActor>(Actor);
		DoorActor->RequiredCondition = "Locked";
		
		for (int32 i = 0; i < ConditionEntry.ConditionID.Num(); ++i)
		{
			const FString& LockID = ConditionEntry.ConditionID[i];
			TObjectPtr<AActor>* FoundLock = LockMap.Find(LockID);
			if (FoundLock)
			{
				TObjectPtr<ALockActor> LockActor = Cast<ALockActor>(*FoundLock);
				if (LockActor)
				{
					// 같은 인덱스의 AdditionalData 값을 사용
					if (ConditionEntry.AdditionalData.IsValidIndex(i))
					{
						LockActor->Password = ConditionEntry.AdditionalData[i];
					}
					LockActor->TargetDoor = DoorActor;
				}
			}
		}
	}
	else if (ConditionEntry.ConditionType == "Item")
	{
		ADoorActor* DoorActor = Cast<ADoorActor>(Actor);
		DoorActor->RequiredCondition = "Item";
		for(const FString& ItemID : ConditionEntry.ConditionID)
		{
			DoorActor->RequiredItem.Add(ItemID);
		}
	}
	else if (ConditionEntry.ConditionType == "Default")
	{
		ADoorActor* DoorActor = Cast<ADoorActor>(Actor);
		DoorActor->RequiredCondition = "Default";
	}
	else if (ConditionEntry.ConditionType == "Button")
	{
		ADoorActor* DoorActor = Cast<ADoorActor>(Actor);
		DoorActor->RequiredCondition = "Button";
		for(int32 i = 0 ; i <  ConditionEntry.ConditionID.Num(); ++i)
		{
			const FString ButtonName = ConditionEntry.ConditionID[i];
			TObjectPtr<AActor>* FindActor = ButtonMap.Find(ButtonName);
			if(FindActor)
			{
				AButtonActor* ButtonActor = Cast<AButtonActor>(*FindActor);
				ButtonActor->DoorActor = DoorActor;
				if(!ConditionEntry.AdditionalData.IsEmpty())
				{
					ButtonActor->RequiredItem.Add(ConditionEntry.AdditionalData[i]);
				}
			}
		}
	}
	else if (ConditionEntry.ConditionType == "Dialogue")
	{
	
	}
	else if (ConditionEntry.ConditionType == "Location")
	{
		
	}
}