#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "EnemyDefinition.generated.h"

/*
 * @brief : 적 캐릭터 스펙 데이터
 * @details :
 * 	- 감지/공격 반경과 이동/공격 정책을 정의
 */
UCLASS(BlueprintType)
class GAME_API UEnemyDefinition : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	/*
	 * @brief : 플레이어 감지 반경(지면 원형 기준)
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Sense", meta=(ClampMin="0.0"))
	float DetectionRadius = 1200.f;

	/*
	 * @brief : 공격 개시 반경(정지 후 공격)
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Sense", meta=(ClampMin="0.0"))
	float AttackRadius = 200.f;

	/*
	 * @brief : 이동 속도 스펙
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Move")
	float MaxWalkSpeed = 400.f;

	/*
	 * @brief : 사용할 공격 세트 이름 목록(콤보 세트 키)
 * @details : 각 항목은 CharacterDefinition.AttackCombos의 키와 일치해야 함
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Combat")
	TArray<FName> AttackSets;

	/*
	 * @brief : 공격 간 최소/최대 간격(초)
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Combat", meta=(ClampMin="0.0"))
	float AttackIntervalMin = 0.8f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Combat", meta=(ClampMin="0.0"))
	float AttackIntervalMax = 1.6f;
};


