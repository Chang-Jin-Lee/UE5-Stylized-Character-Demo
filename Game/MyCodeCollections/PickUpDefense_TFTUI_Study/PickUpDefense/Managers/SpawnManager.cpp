// Fill out your copyright notice in the Description page of Project Settings.


#include "PickUpDefense/Managers/SpawnManager.h"
#include "PickUpDefense/Stage/SplinePath.h"
#include "PickUpDefense/Units//Enemy/BaseEnemy.h"
#include "PickUpDefense/Stage/StageInfoData.h"
#include "PickUpDefense/Units/PlayerUnit/BaseUnit.h"
#include "CJLibrary/DataTable/DataManager/CJDataTableMgr.h"

void USpawnManager::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
}

void USpawnManager::SpawnUnit(const EUnitAttributeType InUnitType, const int32 InLevel, const FVector SpawnLocation, const FRotator SpawnRotation) const
{
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	
	if(ABaseUnit* SpawnedUnit = GetWorld()->SpawnActor<ABaseUnit>(ABaseUnit::StaticClass(), SpawnLocation, SpawnRotation, SpawnParams))
	{
		SpawnedUnit->InitializeUnit(InUnitType, InLevel);

		UE_LOG(LogTemp, Warning, TEXT("Spawn Location : %s"), *SpawnLocation.ToString());
	}
}

void USpawnManager::SpawnEnemyByWave(const FWaveInfo& WaveInfo)
{
	CurrentWaveInfo = &WaveInfo;
	CurrentEnemyIndex = 0;
	SpawnedEnemyCount = 0;
	
	SpawningEnemy();
}

void USpawnManager::SpawningEnemy()
{
	const FEnemySpawnInfo& SpawnPackage = CurrentWaveInfo->EnemyToSpawn[CurrentEnemyIndex];
	
	SpawnedEnemyCount++;
	
	FActorSpawnParameters SpawnParams;
	if(ABaseEnemy* SpawnedPackage = GetWorld()->SpawnActor<ABaseEnemy>(ABaseEnemy::StaticClass(), SpawnParams))
	{
		SpawnedPackage->SetSplinePath(SplinePath);
		SpawnedPackage->InitializeEnemy(SpawnPackage.AttributeType, SpawnPackage.EnemyRankType);
	}
	
	/* 현재 적 종류가 다 생성 되었으면 다음 적으로 이동 */
	if(SpawnedEnemyCount >= SpawnPackage.EnemyPerWave)
	{		
		CurrentEnemyIndex++;
		SpawnedEnemyCount = 0;
		
		if(CurrentEnemyIndex >= CurrentWaveInfo->EnemyToSpawn.Num())
		{
			return; 
		}
	}
	
	GetWorld()->GetTimerManager().SetTimer(SpawnTimerHandle, this, &USpawnManager::SpawningEnemy, SpawnInterval, false);
}

void USpawnManager::CreateSplinePath()
{
	FActorSpawnParameters SpawnParams;
	
	SplinePath = GetWorld()->SpawnActor<ASplinePath>(ASplinePath::StaticClass(), SpawnParams);
}

