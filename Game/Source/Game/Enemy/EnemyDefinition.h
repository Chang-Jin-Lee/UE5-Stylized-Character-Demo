#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "EnemyDefinition.generated.h"

/*
 * @brief : �� ĳ���� ���� ������
 * @details :
 * 	- ����/���� �ݰ�� �̵�/���� ��å�� ����
 */
UCLASS(BlueprintType)
class GAME_API UEnemyDefinition : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	/*
	 * @brief : �÷��̾� ���� �ݰ�(���� ���� ����)
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Sense", meta=(ClampMin="0.0"))
	float DetectionRadius = 1200.f;

	/*
	 * @brief : ���� ���� �ݰ�(���� �� ����)
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Sense", meta=(ClampMin="0.0"))
	float AttackRadius = 200.f;

	/*
	 * @brief : �̵� �ӵ� ����
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Move")
	float MaxWalkSpeed = 400.f;

	/*
	 * @brief : ����� ���� ��Ʈ �̸� ���(�޺� ��Ʈ Ű)
 * @details : �� �׸��� CharacterDefinition.AttackCombos�� Ű�� ��ġ�ؾ� ��
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Combat")
	TArray<FName> AttackSets;

	/*
	 * @brief : ���� �� �ּ�/�ִ� ����(��)
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Combat", meta=(ClampMin="0.0"))
	float AttackIntervalMin = 0.8f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Combat", meta=(ClampMin="0.0"))
	float AttackIntervalMax = 1.6f;
};


