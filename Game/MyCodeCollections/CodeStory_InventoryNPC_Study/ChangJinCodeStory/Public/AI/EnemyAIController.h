#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISense_Sight.h"
#include "EnemyAIController.generated.h"

UCLASS()
class CHANGJINCODESTORY_API AEnemyAIController : public AAIController
{
	GENERATED_BODY()
	
public:
	AEnemyAIController();

	UPROPERTY(EditAnywhere)
	float DistanceFromPlayer = 100;

	virtual void OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result) override;
	
protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
	UAIPerceptionComponent* AIPerceptionComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
	UAISenseConfig_Sight* SightConfig;

	UFUNCTION()
	void OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus);

	void MoveToTarget(AActor* TargetActor);

private:
	FTimerHandle MoveToPlayerTimerHandle;

	void StartFollowingPlayer(AActor* PlayerActor);
	void StopFollowingPlayer();
	void UpdateAnimInstance(bool bIsMoving, int32 GroundSpeed);
	FVector InitialSpawnLocation;
};