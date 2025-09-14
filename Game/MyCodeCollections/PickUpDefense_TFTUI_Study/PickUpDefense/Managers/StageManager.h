// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CombatEventManager.h"
#include "SpawnManager.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "PickUpDefense/Stage/StageInfoData.h"
#include "StageManager.generated.h"


/**
 * 
 */
UENUM(BlueprintType)
enum class EStageState : uint8
{
	Preparation,
	Inprogress,
	Completed,
	CompletedAllStage
};

UENUM(BlueprintType)
enum class EWaveState : uint8
{
	Preparation,
	Inprogress,
	Completed,
	CompletedAllWave
};



UCLASS()
class PICKUPDEFENSE_API UStageManager : public UGameInstanceSubsystem
{
	GENERATED_BODY()


public:
	
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	/* 스테이지 데이터 로딩 */
	void LoadStageInfoDataTable(FName StageName);
	
	/* 현재 스테이지에 대한 정보를 가져옴 */
	UFUNCTION(BlueprintCallable)
	void LoadStageInfo();
	
	/* 스테이지 시작 */
	UFUNCTION(BlueprintCallable)
	void StartStage();

	/* 스테이지 준비 */
	UFUNCTION(BlueprintCallable)
	void PrepareStage();

	/* 스테이지 종료 */
	UFUNCTION(BlueprintCallable)
	void CompleteStage();

	/* 웨이브 시작 */
	UFUNCTION(BlueprintCallable)
	void StartWave();
	
	/* 웨이브 준비 */
	UFUNCTION(BlueprintCallable)
	void PrepareWave();

	/* 웨이브 종료 */
	UFUNCTION(BlueprintCallable)
	void CompleteWave();

	
	 
	UFUNCTION()
	void OnEnemyDestroyed(AActor* PackageActor, bool bCanReceiveReward, int32 RewardWage);

	UFUNCTION(BlueprintCallable, Category="Stage Management")
	EStageState GetStageState() const { return StageState; }

	UFUNCTION(BlueprintCallable, Category="Wave Management")
	EWaveState GetWaveState() const { return WaveState; }
	
	int32 CurrentStageIndex; 

	int32 CurrentWaveIndex; 

	int32 TotalStageCount;
	
	int32 TotalWaveCount;

	UPROPERTY(EditAnywhere)
	float SpawnInterval = 0.5f;

	FTimerHandle SpawnTimerHandle;

	
private:
	
	UPROPERTY()
	UDataTable* StageDataTable = nullptr;

	FStageInfoData* CurrentStageInfo = nullptr;

	UPROPERTY()
	USpawnManager* SpawnManager = nullptr;

	UPROPERTY()
	UCombatEventManager* CombatEventManager = nullptr;
	
	int32 CurrentEnemyIndex;
	int32 SpawnedEnemyListIndex; 
	int32 SpawnedEnemyCount;
	
	int32 TotalSpawnedEnemy;
	
	void StartSpawningEnemy();


	EStageState StageState;
	EWaveState WaveState;

	void SetStageState(EStageState NewState);
	void SetWaveState(EWaveState NewState);



	TMap<FName, FString> StageDataTablePath = {
		{TEXT("Stage1"), TEXT("/Game/Stage/Stage1InfoDataTable.Stage1InfoDataTable")},
		{TEXT("Stage2"), TEXT("/Game/Stage/Stage2InfoDataTable.Stage2InfoDataTable")},
		{TEXT("Stage3"), TEXT("/Game/Stage/Stage3InfoDataTable.Stage3InfoDataTable")}
	};
};
