// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "UnitStats.h"
#include "PickUpDefense/Units/Enemy/BaseEnemy.h"
#include "BaseUnit.generated.h"

class ABaseEnemy;
class USphereComponent;

UCLASS()
class PICKUPDEFENSE_API ABaseUnit : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABaseUnit();

	UPROPERTY()
	USceneComponent* SceneComponent;
	
	UPROPERTY(EditAnywhere)
	USkeletalMeshComponent* BaseSkeletalMeshComponent;

	UPROPERTY()
	USphereComponent* SphereComponent;

private:
	UFUNCTION()
	void OnMeshClicked(UPrimitiveComponent* ClickedComp, FKey ButtonPressed);

protected:
	/* 유닛 스탯 관리 */
	FUnitStats UnitStats;

	EUnitAttributeType UnitAttributeType;
	// 이 부분 적에 대한 정보와 유닛에 대한 정보를 혼동해서 사용한것 같아 해결해둠
	EEnemyAttributeType EnemyAttributeType;
	int32 CurrentLevel;
	int32 MaxUpgradeLevel;
	
	float AttackSpeed;
	float AttackPhysicalPower;
	float AttackMagicalPower;
	float AttackRange;
	float CriticalChance;
	float MissChance;
	float CriticalMultiplier;
	int32 BuyPrice; 
	int32 UpgradeCost;
	int32 SellPrice;
	
	bool bIsAttacking = false;

	UPROPERTY()
	ABaseEnemy* TargetEnemy = nullptr;

	TSet<ABaseEnemy*> EnemiesInRage;
	TQueue<ABaseEnemy*> EnemiesInQueue;
	
	void SetStats();

	ABaseEnemy* FindTarget();
	
	FTimerHandle AttackTimerHandle;

	mutable FRandomStream RandomStream;
	int32 RandomSeed;
	
	void Attack();
	void StartAttack();
	void StopAttack();
	void ApplyDamage() const;
	
public:	

	/* 유닛 정보 */
	int32 GetBuyPrice() const { return BuyPrice; }	// 구매 가격
	int32 GetSellPrice() const { return SellPrice; }		// 판매 가격
	int32 GetCurrentLevel() const { return CurrentLevel; }	// 현재 레벨
	int32 GetUpgradeCost() const { return UpgradeCost; }	// 업그레이드 가격
	bool CanUpgrade() const;								// 업그레이드 가능 여부
	
	void InitializeUnit(EUnitAttributeType InType, int32 InLevel);
	void UpgradeUnit();

	UFUNCTION()
	void HandleEnemyDestroyed(const ABaseEnemy* DestroyedPackage);
	
	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	
};
