#include "EnemyStats.h"

#include "PickUpDefense/Units/PlayerUnit/UnitStats.h"


// 적 속성 & 등급에 따른 스탯 설정
const TMap<TPair<EEnemyAttributeType, EEnemyRankType>, FBaseEnemyStats> FEnemyStats::EnemyStatsData = {
	/* Type1의 각 등급별 스탯 */
	{
		TPair<EEnemyAttributeType, EEnemyRankType>(EEnemyAttributeType::Type1, EEnemyRankType::Basic),
		FBaseEnemyStats(5.f, 1.f, 1.f, 1.f, 1, FVector(1.f))
	},
	{
		TPair<EEnemyAttributeType, EEnemyRankType>(EEnemyAttributeType::Type1, EEnemyRankType::MidBoss),
		FBaseEnemyStats(5.f, 5.f, 5.f, 1.f, 3, FVector(1.5f))
	},
	{
		TPair<EEnemyAttributeType, EEnemyRankType>(EEnemyAttributeType::Type1, EEnemyRankType::Boss),
		FBaseEnemyStats(5.f, 10.f, 10.f, 1.f, 10, FVector(2.f))
	},

	/* Type2의 각 등급별 스탯 */
	{
		TPair<EEnemyAttributeType, EEnemyRankType>(EEnemyAttributeType::Type2, EEnemyRankType::Basic),
		FBaseEnemyStats(5.f, 1.f, 1.f, 1.f, 1, FVector(1.f))
	},
	{
		TPair<EEnemyAttributeType, EEnemyRankType>(EEnemyAttributeType::Type2, EEnemyRankType::MidBoss),
		FBaseEnemyStats(10.f, 5.f,  5.f, 1.f, 3, FVector(1.5f))
	},
	{
		TPair<EEnemyAttributeType, EEnemyRankType>(EEnemyAttributeType::Type2, EEnemyRankType::Boss),
		FBaseEnemyStats(20.f, 10.f, 10.f, 1.f, 10, FVector(2.f))
	},

	/* Type3의 각 등급별 스탯 */
	{
		TPair<EEnemyAttributeType, EEnemyRankType>(EEnemyAttributeType::Type3, EEnemyRankType::Basic),
		FBaseEnemyStats(5.f, 1.f, 1.f,  1.f, 1, FVector(1.f))
	},
	{
		TPair<EEnemyAttributeType, EEnemyRankType>(EEnemyAttributeType::Type3, EEnemyRankType::MidBoss),
		FBaseEnemyStats(10.f, 5.f, 5.f, 1.f, 3, FVector(1.5f))
	},
	{
		TPair<EEnemyAttributeType, EEnemyRankType>(EEnemyAttributeType::Type3, EEnemyRankType::Boss),
		FBaseEnemyStats(20.f, 10.f, 10.f, 1.f, 10, FVector(2.f))
	},
};


/* 적 타입에 따라 Mesh설정 */
TMap<EEnemyAttributeType, UStaticMesh*> FEnemyStats::EnemyMeshData = {
	{
		EEnemyAttributeType::Type1, {
			LoadObject<UStaticMesh>(nullptr, TEXT("/Engine/BasicShapes/Cube"))
		}
	},
	{
		EEnemyAttributeType::Type2, {
			LoadObject<UStaticMesh>(nullptr, TEXT("/Engine/BasicShapes/Cone"))
		}
	},
	{
		EEnemyAttributeType::Type3, {
			LoadObject<UStaticMesh>(nullptr, TEXT("/Engine/BasicShapes/Sphere"))
		}
	}
};
