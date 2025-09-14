#pragma once

#include "CoreMinimal.h"
#include "EnemyStats.generated.h"

UENUM(BlueprintType)
enum class EEnemyAttributeType : uint8
{
	Type1,
	Type2,
	Type3
};

UENUM(BlueprintType)
enum class EEnemyRankType : uint8
{
	Basic,
	MidBoss,
	Boss
};


USTRUCT()
struct FBaseEnemyStats
{
	GENERATED_BODY()

	float HP;
	float PhysicalDefence;
	float MagicalDefence;
	float Speed;
	int32 RewardWage;
	
	FVector MeshScale;
};


USTRUCT()
struct FEnemyStats
{
	GENERATED_BODY()

	// 적 속성 & 등급에 따른 스탯 설정
	static const TMap<TPair<EEnemyAttributeType, EEnemyRankType>, FBaseEnemyStats> EnemyStatsData;
	
	static const FBaseEnemyStats* GetStatData(EEnemyAttributeType InAttributeType, EEnemyRankType InRankType)
	{
		if(const FBaseEnemyStats* StatData = EnemyStatsData.Find(TPair<EEnemyAttributeType, EEnemyRankType> (InAttributeType,  InRankType)))
		{
			return StatData;
		}
		return nullptr;
	}

	
	// 적 속성에 따른 매쉬 설정
	static TMap<EEnemyAttributeType, UStaticMesh*> EnemyMeshData;
	
	static UStaticMesh* GetMesh(const EEnemyAttributeType InAttributeType)
	{
		if(UStaticMesh** FoundMesh = EnemyMeshData.Find(InAttributeType))
		{
			return *FoundMesh;
		}
		return nullptr;
	}
	
};
