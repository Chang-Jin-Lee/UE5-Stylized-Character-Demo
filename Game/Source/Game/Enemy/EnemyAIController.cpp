#include "Game/Enemy/EnemyAIController.h"
#include "Game/Enemy/EnemyCharacter.h"
#include "Game/Enemy/EnemyDefinition.h"
#include "Game/Combat/CharacterCombatComponent.h"
#include "Kismet/GameplayStatics.h"
#include "NavigationSystem.h"
#include "GameFramework/CharacterMovementComponent.h"

AEnemyAIController::AEnemyAIController()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AEnemyAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	Enemy = Cast<AEnemyCharacter>(InPawn);
	Spec = (Enemy && Enemy->EnemySpec.IsValid()) ? Enemy->EnemySpec.LoadSynchronous() : nullptr;
}

void AEnemyAIController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	if (!Enemy || !Spec) return;

	UpdateTarget();
	UpdateState(DeltaSeconds);
}

void AEnemyAIController::UpdateTarget()
{
	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(this, 0);
	TargetActor = PlayerPawn;
}

bool AEnemyAIController::IsInDetectionRange(const FVector& MyLoc, const FVector& TargetLoc) const
{
	return FVector::Dist2D(MyLoc, TargetLoc) <= Spec->DetectionRadius;
}

bool AEnemyAIController::IsInAttackRange(const FVector& MyLoc, const FVector& TargetLoc) const
{
	return FVector::Dist2D(MyLoc, TargetLoc) <= Spec->AttackRadius;
}

void AEnemyAIController::UpdateState(float DeltaSeconds)
{
	if (!TargetActor.IsValid()) return;

	const FVector MyLoc = Enemy->GetActorLocation();
	const FVector TargetLoc = TargetActor->GetActorLocation();

	if (!IsInDetectionRange(MyLoc, TargetLoc))
	{
		StopMovement();
		if (Enemy && Enemy->GetCharacterMovement())
		{
			Enemy->GetCharacterMovement()->StopMovementImmediately();
		}
		return; // Idle
	}

	if (!IsInAttackRange(MyLoc, TargetLoc))
	{
		// 실제 이동 입력으로 가속 → AnimBP가 속도로 전이하도록
		if (Enemy && Enemy->GetCharacterMovement())
		{
			Enemy->GetCharacterMovement()->MaxWalkSpeed = Spec->MaxWalkSpeed;
			const FVector ToTarget = TargetLoc - MyLoc;
			const FRotator DesiredYaw(0.f, ToTarget.Rotation().Yaw, 0.f);
			Enemy->SetActorRotation(DesiredYaw);
			Enemy->AddMovementInput(DesiredYaw.Vector(), 1.0f);
		}
		return; // Chase
	}

	// Attack
	StopMovement();
	if (Enemy && Enemy->GetCharacterMovement())
	{
		Enemy->GetCharacterMovement()->StopMovementImmediately();
	}
	TimeUntilNextAttack -= DeltaSeconds;
	if (TimeUntilNextAttack <= 0.f)
	{
		DoAttack();
		TimeUntilNextAttack = FMath::RandRange(Spec->AttackIntervalMin, Spec->AttackIntervalMax);
	}
}

void AEnemyAIController::DoAttack()
{
	if (!Enemy) return;
	if (UCharacterCombatComponent* Combat = Enemy->FindComponentByClass<UCharacterCombatComponent>())
	{
		// 무작위 공격 세트 선택
		FName AttackSet = NAME_None;
		if (Spec && Spec->AttackSets.Num() > 0)
		{
			AttackSet = Spec->AttackSets[FMath::RandRange(0, Spec->AttackSets.Num() - 1)];
		}
		Combat->TryAttackBySet(AttackSet);
	}
}


