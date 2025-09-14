#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "PickUpDefense/Units/Enemy/EnemyStats.h"
#include "StageInfoData.generated.h"


USTRUCT()
struct FEnemySpawnInfo
{
	GENERATED_BODY()

	/* 적 속성 */
	UPROPERTY(EditAnywhere, Category="Enemy")
	EEnemyAttributeType AttributeType;

	/* 적 등급 */
	UPROPERTY(EditAnywhere, Category="Enemy")
	EEnemyRankType EnemyRankType;

	/* 웨이브 당 출현하는 적의 수 */
	UPROPERTY(EditAnywhere, Category="Enemy")
	int32 EnemyPerWave;
};


USTRUCT()
struct FWaveInfo
{
	GENERATED_BODY()

	/* 웨이브에 생성되는 적의 동류와 수 */
	UPROPERTY(EditAnywhere, Category="Enemy")
	TArray<FEnemySpawnInfo> EnemyToSpawn;

	/* 웨이브간 휴식 시간 (초 단위) */
	UPROPERTY(EditAnywhere, Category="Enemy")
	float BreakTime; 
};


USTRUCT(BlueprintType)
struct FStageInfoData : public FTableRowBase
{
	GENERATED_BODY()

	/* 스테이지 인덱스 */
	UPROPERTY(EditAnywhere, Category="Enemy")
	int32 StageIndex;

	/* 해당 스테이지의 모든 웨이브 정보 */
	UPROPERTY(EditAnywhere, Category="Enemy")
	TArray<FWaveInfo> Waves;
	
};
