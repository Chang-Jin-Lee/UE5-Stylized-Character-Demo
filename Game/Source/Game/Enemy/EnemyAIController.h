#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "EnemyAIController.generated.h"

class AEnemyCharacter;
class UEnemyDefinition;

/*
 * @brief : 적 AI 컨트롤러 (Idle/Chase/Attack)
 */
UCLASS()
class GAME_API AEnemyAIController : public AAIController
{
	GENERATED_BODY()

public:
	AEnemyAIController();
	virtual void OnPossess(APawn* InPawn) override;
	virtual void Tick(float DeltaSeconds) override;

protected:
	UPROPERTY()
	AEnemyCharacter* Enemy;

	UPROPERTY()
	const UEnemyDefinition* Spec;

	TWeakObjectPtr<AActor> TargetActor;
	float TimeUntilNextAttack = 0.f;

private:
	void UpdateTarget();
	void UpdateState(float DeltaSeconds);
	bool IsInDetectionRange(const FVector& MyLoc, const FVector& TargetLoc) const;
	bool IsInAttackRange(const FVector& MyLoc, const FVector& TargetLoc) const;
	void DoAttack();
};


