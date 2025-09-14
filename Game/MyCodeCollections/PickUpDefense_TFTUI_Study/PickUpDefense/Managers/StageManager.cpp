// Fill out your copyright notice in the Description page of Project Settings.


#include "PickUpDefense/Managers/StageManager.h"
#include "CombatEventManager.h"
#include "SpawnManager.h"

void UStageManager::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	
	Collection.InitializeDependency(USpawnManager::StaticClass());
	Collection.InitializeDependency(UCombatEventManager::StaticClass());
	
	// SpawnManager 초기화
	SpawnManager = GetGameInstance()->GetSubsystem<USpawnManager>();
	// CombatEventManager 초기화
	CombatEventManager = GetGameInstance()->GetSubsystem<UCombatEventManager>();
	
	if(CombatEventManager)
	{
		CombatEventManager->OnGlobalEnemyDestroyed.AddDynamic(this, &UStageManager::OnEnemyDestroyed);
	}
}


void UStageManager::LoadStageInfoDataTable(FName StageName)
{
	if(StageDataTablePath.Contains(StageName))
	{
		FString DataTablePath = StageDataTablePath[StageName];
		FString FullPath = FString::Printf(TEXT("DataTable'%s'"), *DataTablePath);
		
		StageDataTable = LoadObject<UDataTable>(nullptr, *FullPath);

		if(StageDataTable)
		{
			TArray<FName> RowNames = StageDataTable->GetRowNames();
			TotalStageCount = RowNames.Num();
			CurrentStageIndex = 0;
			UE_LOG(LogTemp, Warning, TEXT("StageDataTable!"));
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("No StageDataTable!"));
		}
	}
}

void UStageManager::LoadStageInfo()
{
	if(StageDataTable && CurrentStageIndex <= TotalStageCount)
	{
		FString ContextString;
		
		FName RowName = FName(*FString::Printf(TEXT("Stage%d"), CurrentStageIndex));
		FStageInfoData* LoadedData = StageDataTable->FindRow<FStageInfoData>(RowName, ContextString);

		if(LoadedData)
		{
			CurrentStageInfo = LoadedData;
			TotalWaveCount = CurrentStageInfo->Waves.Num();
			CurrentWaveIndex = 0;

			UE_LOG(LogTemp, Warning, TEXT("Loaded Data!"));
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("No Loaded Data!"));
		}
	}
}

void UStageManager::PrepareStage()
{
	SetStageState(EStageState::Preparation);

	LoadStageInfoDataTable("Stage1");
	
	CurrentStageIndex = CurrentStageIndex + 1;

	UE_LOG(LogTemp, Warning, TEXT("Prepare Stage! %d // total count : %d"), CurrentStageIndex, TotalStageCount);
	
	if(CurrentStageIndex <= TotalStageCount)
	{
		UE_LOG(LogTemp, Warning, TEXT("LoadStageInfo!"));
		
		LoadStageInfo();
	}
	
	/* Path 생성 요청 */
	SpawnManager->CreateSplinePath();
}

void UStageManager::StartStage()
{
	SetStageState(EStageState::Inprogress);
	
	UE_LOG(LogTemp, Warning, TEXT("Start Stage!"));
}

void UStageManager::CompleteStage()
{
	SetStageState(EStageState::Completed);
	
}

void UStageManager::PrepareWave()
{
	SetWaveState(EWaveState::Preparation);
}

void UStageManager::StartWave()
{
	SetWaveState(EWaveState::Inprogress);

	UE_LOG(LogTemp, Warning, TEXT("Start Wave~!"));
	
	if(SpawnManager && CurrentWaveIndex <= TotalWaveCount)
	{
		TotalSpawnedEnemy = 0;

		const FWaveInfo& CurrentWave = CurrentStageInfo->Waves[CurrentWaveIndex];
		
		SpawnManager->SpawnEnemyByWave(CurrentWave);

		for(const auto& [DeliveryType, PackageType, EnemyPerWave] : CurrentWave.EnemyToSpawn)
		{
			TotalSpawnedEnemy += EnemyPerWave;
		}
	}
}

void UStageManager::CompleteWave()
{
	SetWaveState(EWaveState::Completed);
	
	// 웨이브 끝남 -> 다음 웨이브 준비
	CurrentWaveIndex++;

	if(CurrentWaveIndex >= TotalWaveCount)
	{
		PrepareWave();
	}
	else
	{
		// 다음 웨이브가 없다면 스테이지 종료
		SetWaveState(EWaveState::CompletedAllWave);
		CompleteStage();
	}
}


void UStageManager::OnEnemyDestroyed(AActor*, bool, int32 )
{
	TotalSpawnedEnemy--;

	if(TotalSpawnedEnemy == 0)
	{
		CompleteWave();
	}
}


void UStageManager::SetStageState(const EStageState NewState)
{
	StageState = NewState;


	FString String = StaticEnum<EStageState>()->GetValueAsString(NewState);
	UE_LOG(LogTemp, Warning, TEXT("Current Stage State : %s!"), *String);
}

void UStageManager::SetWaveState(const EWaveState NewState)
{
	WaveState = NewState;
	
	FString String = StaticEnum<EWaveState>()->GetValueAsString(NewState);
	UE_LOG(LogTemp, Warning, TEXT("Current Stage State : %s!"), *String);
}
