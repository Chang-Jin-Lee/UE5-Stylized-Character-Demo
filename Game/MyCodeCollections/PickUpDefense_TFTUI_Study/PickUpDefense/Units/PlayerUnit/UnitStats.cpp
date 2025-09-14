#include "UnitStats.h"


const TMap<EUnitAttributeType, TMap<int32, FBaseUnitStats>> FUnitStats::UnitTypeLevelUpgradeStats = {
	{
		EUnitAttributeType::Ahri, {
		{1, FBaseUnitStats(1.f, 0.f, 1.f, 3.f, 0.1f, 0.15f, 3, 1, 3)},
		{2, FBaseUnitStats(3.f, 0.f, 3.f, 4.f, 0.1f, 0.15f, 3, 4, 5)},
		{3, FBaseUnitStats(5.f, 0.f, 5.f, 5.f, 0.15f, 0.1f, 3, 9, 0)}
		}
	},
	{
		EUnitAttributeType::Lux, {
			{1, FBaseUnitStats(1.f, 0.f, 1.f, 3.f, 0.1f, 0.15f,3, 1, 3)},
			{2, FBaseUnitStats(3.f, 0.f, 3.f, 4.f, 0.1f, 0.15f, 3, 4, 5)},
			{3, FBaseUnitStats(5.f, 0.f, 5.f, 5.f, 0.1f, 0.15f, 3, 9, 0)}
		}
	},
	{
		EUnitAttributeType::Jinx, {
		{1, FBaseUnitStats(1.f, 1.f, 0.f, 3.f, 0.1f, 0.15f,3, 1,3)},
		{2, FBaseUnitStats(3.f, 3.f, 0.f, 4.f, 0.1f, 0.15f,3, 4,5)},
		{3, FBaseUnitStats(5.f, 5.f, 0.f, 5.f, 0.1f, 0.15f,3,9, 0)}
		}
	},
	{
		EUnitAttributeType::Jhin, {
			{1, FBaseUnitStats(1.f, 1.f, 0.f, 3.f, 0.1f, 0.15f,3, 1, 3)},
			{2, FBaseUnitStats(3.f, 3.f, 0.f, 4.f, 0.1f, 0.15f, 3, 4, 5)},
			{3, FBaseUnitStats(5.f, 5.f, 0.f, 5.f, 0.1f, 0.15f, 3, 9, 0)}
		}
	},
	{
		EUnitAttributeType::Lucian, {
			{1, FBaseUnitStats(1.f, 1.f, 1.f, 3.f, 0.1f, 0.15f,3, 1, 3)},
			{2, FBaseUnitStats(3.f, 3.f, 2.f, 4.f, 0.1f, 0.15f, 3, 4, 5)},
			{3, FBaseUnitStats(5.f, 5.f, 3.f, 5.f, 0.1f, 0.15f, 3, 9, 0)}
		}
	},
	{
		EUnitAttributeType::MissFortune, {
				{1, FBaseUnitStats(1.f, 1.f, 1.f, 3.f, 0.1f, 0.15f,3, 1, 3)},
				{2, FBaseUnitStats(3.f, 3.f, 2.f, 4.f, 0.1f, 0.15f, 3, 4, 5)},
				{3, FBaseUnitStats(5.f, 5.f, 3.f, 5.f, 0.1f, 0.15f, 3, 9, 0)}
		}
	},
	{
		EUnitAttributeType::Irelia, {
					{1, FBaseUnitStats(1.f, 1.f, 1.f, 3.f, 0.1f, 0.15f,3, 1, 3)},
					{2, FBaseUnitStats(3.f, 3.f, 2.f, 4.f, 0.1f, 0.15f, 3, 4, 5)},
					{3, FBaseUnitStats(5.f, 5.f, 3.f, 5.f, 0.1f, 0.15f, 3, 9, 0)}
		}
	},
};

int32 FUnitStats::GetMaxUpgradeLevel(const EUnitAttributeType UnitType)
{
	if(const TMap<int32, FBaseUnitStats>* LevelStats = UnitTypeLevelUpgradeStats.Find(UnitType))
	{
		return LevelStats->Num();
	}

	return 0;
}


 const TMap<EUnitAttributeType, FUnitMeshData> FUnitStats::UnitMeshData = {
	{
		EUnitAttributeType::Ahri, {
			TEXT("/Game/Assets/Ahri/Ahri_gltf.Ahri_gltf"),
			TEXT("/Game/Characters/ABP_Ahri.ABP_Ahri_C"),
			FVector(1.f)
		}
	},
	{
		EUnitAttributeType::Jinx, {
			TEXT("/Game/Assets/Ahri/Ahri_gltf.Ahri_gltf"),
			TEXT("/Game/Characters/ABP_Ahri.ABP_Ahri_C"),
			FVector(1.f)
		}
	},
	{
		EUnitAttributeType::Lux, {
			TEXT("/Game/Assets/Ahri/Ahri_gltf.Ahri_gltf"),
			TEXT("/Game/Characters/ABP_Ahri.ABP_Ahri_C"),
			FVector(1.f)
		}
	},
	{
		EUnitAttributeType::Jhin, {
			TEXT("/Game/Assets/Ahri/Ahri_gltf.Ahri_gltf"),
			TEXT("/Game/Characters/ABP_Ahri.ABP_Ahri_C"),
			FVector(1.f)
		}
	},
	{ 
		EUnitAttributeType::Lucian, {
			TEXT("/Game/Assets/Ahri/Ahri_gltf.Ahri_gltf"),
			TEXT("/Game/Characters/ABP_Ahri.ABP_Ahri_C"),
			FVector(1.f)
		}
	},
	{
		EUnitAttributeType::MissFortune, {
			TEXT("/Game/Assets/MissFortune/MissFortune_Complete.MissFortune_Complete"),
			TEXT("/Game/Characters/ABP_MissFortune.ABP_MissFortune_C"),
			FVector(1.f)
		}
	},
	{
		EUnitAttributeType::Irelia, {
			TEXT("/Script/Engine.SkeletalMesh'/Game/Assets/Irelia/irelia_scale1.irelia_scale1'"),
			TEXT("/Game/Characters/ABP_Irelia.ABP_Irelia_C"),
			FVector(1.f)
		}
	}
 };
