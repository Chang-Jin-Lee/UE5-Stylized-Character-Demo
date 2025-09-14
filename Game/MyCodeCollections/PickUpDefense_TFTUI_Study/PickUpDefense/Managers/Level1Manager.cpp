// Fill out your copyright notice in the Description page of Project Settings.


#include "Level1Manager.h"

#include "Engine/World.h"
#include "PickUpDefense/GameInstance/PickUpDefenseGameInstance.h"
#include "PickUpDefense/GameInstance/PickUpDefenseGameInstanceSubsystem.h"

void ULevel1Manager::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	UE_LOG(LogTemp, Warning, TEXT("Level1Manager Initialize"));
}

void ULevel1Manager::Deinitialize()
{
	Super::Deinitialize();
}

void ULevel1Manager::test()
{
	UE_LOG(LogTemp, Warning, TEXT("Level1Manager test"));

	// if(g_PickupDefenseGameInstanceIsValid)
	// {
	// 	UCJDataTableMgr* DataTableMgr = g_PickupDefenseGameInstance->GetDataTableMgr();
	// 	if(DataTableMgr)
	// 	{
	// 		TArray<FLevelDataRow> ParsedData;
	// 		DataTableMgr->LoadDataFromCSV<FLevelDataRow>("LevelManagers", ParsedData);
	// 		UE_LOG(LogTemp, Warning, TEXT("DataTableMgr is ready to use."));
	// 		if(ParsedData.IsEmpty() == false)
	// 		{
	// 			// FString DisplayName = FLevelTableHelper::GetLevelDisplayName(nLevelNameTable::Level1);
	// 			FString CurrentLevelName = GetWorld()->GetMapName();
	// 			
	// 			for(FLevelDataRow row : ParsedData)
	// 			{
	// 				// if(row.LevelName == CurrentLevelName)
	// 				// {
	// 				// UObject* _manager = LoadManagerBlueprintFromName(row.LevelName);
	// 				// if(_manager)
	// 				// {
	// 				// 	UE_LOG(LogTemp, Error, TEXT("_manager->GetName(): %s"), *_manager->GetName());
	// 				// }
	// 				// }
	// 				
	// 				if(UCJGameInstance* gameinstance = Cast<UCJGameInstance>(GetWorld()->GetGameInstance()))
	// 				{
	// 					UE_LOG(LogTemp, Warning, TEXT("gameinstance->GetCurrentLevelName() : %s"), *gameinstance->GetCurrentLevelName());
	// 				}
	// 				else
	// 				{
	// 					UE_LOG(LogTemp, Warning, TEXT("Fail Load gameinstance->GetCurrentLevelName() "));
	// 				}
	// 				
	// 				UE_LOG(LogTemp, Warning, TEXT("row.LevelName, row.ManagerName : %s, %s"), *row.LevelName, *row.ManagerName);
	// 			}
	// 		}
	// 	}
	// }

	
}
