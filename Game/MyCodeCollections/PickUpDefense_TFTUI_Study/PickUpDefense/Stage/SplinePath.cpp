// Fill out your copyright notice in the Description page of Project Settings.


#include "SplinePath.h"
#include "Components/SplineComponent.h"
#include "CoreFramework/GameInstanceSubsystem/CJGameInstanceSubsystem.h"
#include "DataTable/DataManager/CJDataTableMgr.h"
#include "Engine/GameInstance.h"
#include "Engine/World.h"
#include "ManagerRegistry/Data/Level1JsonData.h"
#include "ManagerRegistry/Data/Level2JsonData.h"
#include "PickUpDefense/GameInstance/PickUpDefenseGameInstance.h"
#include "PickUpDefense/GameInstance/PickUpDefenseGameInstanceSubsystem.h"
#include "PickUpDefense/Managers/Level1Manager.h"

// Sets default values
ASplinePath::ASplinePath()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SplineComponent = CreateDefaultSubobject<USplineComponent>(TEXT("SplineComponent"));
	RootComponent = SplineComponent;
	
	SetSplinePath();
}

// Called when the game starts or when spawned
void ASplinePath::BeginPlay()
{
	Super::BeginPlay();

	// if(g_Level1ManagerIsValid)
	// 	g_Level1Manager->test();
	//
	// TArray<FLevel1DataRow> ParsedData1;

	// if(g_PickupDefenseGameInstanceIsValid)
	// {
	// 	UCJDataTableMgr* DataTableMgr = g_PickupDefenseGameInstance->GetDataTableMgr();
	// 	if (DataTableMgr)
	// 	{
	// 		DataTableMgr->LoadDataFromCSVStruct<FLevel1DataRow>(TEXT("Level1Manager"), ParsedData1);
	//
	// 		if (ParsedData1.Num() > 0)
	// 		{
	// 			for (const FLevel1DataRow& Row : ParsedData1)
	// 			{
	// 				UE_LOG(LogTemp, Log, TEXT("Tid: %s, Name: %s, Type: %s, Number: %s"), *Row.Tid, *Row.Name, *Row.Type, *Row.Number);
	// 			}
	// 		}
	// 		else
	// 		{
	// 			UE_LOG(LogTemp, Error, TEXT("Failed to load or parse the CSV file."));
	// 		}
	// 	}
	// }
	
	// if(g_PickupDefenseGameInstanceIsValid)
	// {
	// 	TArray<FLevelDataRow> leveldata = g_PickupDefenseGameInstance->GetLevelData();
	// 	if(leveldata.Num() > 0 )
	// 	{
	// 		for(FLevelDataRow levelDataRow : leveldata)
	// 		{
	// 			TArray<FLevel1ShootRow> Level1ShootRows = g_PickupDefenseGameInstance->GetJsonDatas<FLevel1ShootRow>(levelDataRow.ManagerFileName);
	// 			for(FLevel1ShootRow Level1ShootRow : Level1ShootRows)
	// 			{
	// 				UE_LOG(LogTemp, Warning, TEXT("Level1ShootRows %s, %s, %s"), *Level1ShootRow.Tid, *Level1ShootRow.ItemName, *Level1ShootRow.ATK);
	// 			}
	// 			
	// 		}
	// 		
	// 	}
	// 	
	// }

	if(g_PickupDefenseGameInstanceIsValid)
	{
		TArray<TMap<FString, FString>> _datas =  g_PickupDefenseGameInstance->GetLevelCSVData("Level1Shoot");
		for(TMap<FString, FString> _data : _datas)
		{
			for (const TPair<FString, FString>& pair : _data)
			{
				UE_LOG(LogTemp, Log, TEXT("Key: %s, Value: %s"), *pair.Key, *pair.Value);
			}
		}
		
		TArray<TMap<FString, FString>> _datas2 =  g_PickupDefenseGameInstance->GetLevelCSVData("Level2Manager");
		for(TMap<FString, FString> _data : _datas2)
		{
			for (const TPair<FString, FString>& pair : _data)
			{
				UE_LOG(LogTemp, Log, TEXT("Key: %s, Value: %s"), *pair.Key, *pair.Value);
			}
		}
	}

	// if(g_Level1ManagerIsValid)
	// {
	// 	g_Level1Manager->test();
	// }
	// else
	// {
	// 	UE_LOG(LogTemp, Warning, TEXT("ASplinePath::BeginPlay Error"));
	// }

	// TArray<FLevel2DataRow> ParsedData;

	// UCJGameInstanceSubsystem 인스턴스에 접근하여 GetGlobalManager 호출
	// if (UPickUpDefenseGameInstance* DataTableMgrInstance = GetWorld()->GetGameInstance()->GetSubsystem<UPickUpDefenseGameInstance>())
	// if (UPickUpDefenseGameInstance* DataTableMgrInstance = Cast<UPickUpDefenseGameInstance>(GetWorld()->GetGameInstance()))
	// if(UPickUpDefenseGameInstance* DataTableMgrInstance = Cast<UPickUpDefenseGameInstance>(GetWorld()->GetGameInstance()))
	// {
	// 	UCJDataTableMgr* DataTableMgr = DataTableMgrInstance->GetManager<UCJDataTableMgr>();
	// 	if (DataTableMgr)
	// 	{
	// 		DataTableMgr->LoadDataFromCSVStruct<FLevel2DataRow>(TEXT("Level2Manager"), ParsedData);
	//
	// 		if (ParsedData.Num() > 0)
	// 		{
	// 			for (const FLevel2DataRow& Row : ParsedData)
	// 			{
	// 				UE_LOG(LogTemp, Log, TEXT("Tid: %s, Name: %s, Type: %s, Number: %s"), *Row.Tid, *Row.Name, *Row.Type, *Row.Number);
	// 			}
	// 		}
	// 		else
	// 		{
	// 			UE_LOG(LogTemp, Error, TEXT("Failed to load or parse the CSV file."));
	// 		}
	// 	}
	// }
	
	// UCJDataTableMgr* DataTableMgrInstance = UCJGameInstanceSubsystem::GetGlobalManager<UCJDataTableMgr>();
	// UCJDataTableMgr* DataTableMgrInstance = NewObject<UCJDataTableMgr>();
}

// Called every frame
void ASplinePath::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASplinePath::SetSplinePath()
{
	SplineComponent->ClearSplinePoints();
	
	SplineComponent->AddSplinePoint(FVector(0.f, 0.f, 10.f), ESplineCoordinateSpace::Local, true);
	SplineComponent->AddSplinePoint(FVector(1000.f, 0.f, 10.f), ESplineCoordinateSpace::Local, true);
	SplineComponent->AddSplinePoint(FVector(1000.f, 1000.f, 10.f), ESplineCoordinateSpace::Local, true);
	SplineComponent->AddSplinePoint(FVector(0.f, 1000.f, 10.f), ESplineCoordinateSpace::Local, true);
	
	SplineComponent->SetSplinePointType(0, ESplinePointType::Linear);
	SplineComponent->SetSplinePointType(1, ESplinePointType::Linear);
	SplineComponent->SetSplinePointType(2, ESplinePointType::Linear);
	SplineComponent->SetSplinePointType(3, ESplinePointType::Linear);

	SplineComponent->SetClosedLoop(false);
	
	SplineComponent->UpdateSpline();
}

