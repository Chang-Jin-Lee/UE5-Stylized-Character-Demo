#pragma once

#include "CoreMinimal.h"
#include "PickUpDefense/Units/Animation/CharacterAnimInstance.h"
#include "UnitStats.generated.h"

UENUM(BlueprintType)
enum class EUnitAttributeType : uint8
{
	Ahri,
	Lux,
	Jinx,
	Jhin,
	Lucian,
	MissFortune,
	Irelia
};

UENUM(BlueprintType)
enum class EDamageType : uint8
{
	Normal,
	Critical
};

USTRUCT()
struct FBaseUnitStats
{
	GENERATED_BODY()
	
	float AttackSpeed;
	float AttackPhysicalPower;
	float AttackMagicalPower;
	float AttackRange;
	float CriticalChance;
	float MissChance;
	float CriticalMultiplier;
	int32 BuyPrice;
	int32 UpgradeCost;
};

USTRUCT()
struct FUnitMeshData
{
	GENERATED_BODY()

	UPROPERTY()
	FName UnitSkeletalMeshDir;

	UPROPERTY()
	FName AnimInstanceDir;
		
	UPROPERTY()
	FVector MeshScale;
};


USTRUCT()
struct FUnitStats
{
	GENERATED_BODY()
	
	static const TMap<EUnitAttributeType, TMap<int32, FBaseUnitStats>> UnitTypeLevelUpgradeStats;

	static int32 GetMaxUpgradeLevel(EUnitAttributeType VehicleType);

	static const TMap<EUnitAttributeType, FUnitMeshData> UnitMeshData;
	static const FUnitMeshData* GetMeshData(const EUnitAttributeType VehicleType)
	{
		return UnitMeshData.Find(VehicleType);
	}
};