// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "CombatEventManager.generated.h"

/**
 * 
 */

/* 웨이브 시작 이벤트 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnStartWave, int32, CurrentStage, int32, CurrentWage);


/* 적 피격 이벤트 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnGlobalEnemyDamaged, AActor*, EnemyActor, int32, DamageAmount, int32, CurrentHP);

/* 적 파괴 이벤트 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnGlobalEnemyDestroyed, AActor*, EnemyActor, bool, bCanReceiveReward, int32, RewardWage);


UCLASS()
class PICKUPDEFENSE_API UCombatEventManager : public UGameInstanceSubsystem
{
	GENERATED_BODY()


public:

	UPROPERTY()
	FOnStartWave OnStartWave;

	
	/* 적 피격 이벤트 피해량, 현재 HP */
	UPROPERTY()
	FOnGlobalEnemyDamaged OnGlobalEnemyDamaged; 

	/* 적 파괴 이벤트  */
	UPROPERTY()
	FOnGlobalEnemyDestroyed OnGlobalEnemyDestroyed;
	
};
