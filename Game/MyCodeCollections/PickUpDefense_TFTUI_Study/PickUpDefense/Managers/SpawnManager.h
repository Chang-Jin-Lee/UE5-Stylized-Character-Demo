// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PickUpDefense/Units/PlayerUnit/UnitStats.h"
#include "Subsystems/GameInstanceSubsystem.h" 
#include "SpawnManager.generated.h"

struct FWaveInfo;
class ASplinePath;
enum class EEnemyRankType : uint8;
enum class EEnemyAttributeType : uint8;

UCLASS()
class PICKUPDEFENSE_API USpawnManager : public UGameInstanceSubsystem  
{
	GENERATED_BODY()
	
public:	

	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	

	UFUNCTION(BlueprintCallable, Category="Spawn")
	void SpawnUnit(EUnitAttributeType InUnitType, int32 InLevel, FVector SpawnLocation, FRotator SpawnRotation) const;

	UFUNCTION()
	void SpawnEnemyByWave(const FWaveInfo& WaveInfo);

	UFUNCTION()
	void SpawningEnemy();
	
	UFUNCTION(BlueprintCallable, Category="Spawn")
	void CreateSplinePath();


	UPROPERTY()
	ASplinePath* SplinePath;


	UPROPERTY(EditAnywhere)
	float SpawnInterval = 0.5f;

	FTimerHandle SpawnTimerHandle;
	
private:
	const FWaveInfo* CurrentWaveInfo = nullptr;
	int32 CurrentEnemyIndex;
	int32 SpawnedEnemyCount;
};
