#include "AI/EnemyAIController.h"

#include "Animation/PlayerAnimInstance.h"
#include "Characters/EnemyCharacter.h"
#include "Characters/PlayerCharacter.h"
#include "Components/CapsuleComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AISense_Sight.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "GameFramework/CharacterMovementComponent.h"

AEnemyAIController::AEnemyAIController()
{
    AIPerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerceptionComponent"));
    SetPerceptionComponent(*AIPerceptionComponent);

    SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SightConfig"));
    SightConfig->SightRadius = 300.0f;
    SightConfig->LoseSightRadius = SightConfig->SightRadius + DistanceFromPlayer;
    SightConfig->PeripheralVisionAngleDegrees = 90.0f;
    SightConfig->SetMaxAge(5.0f);
    SightConfig->AutoSuccessRangeFromLastSeenLocation = 420.0f;

    SightConfig->DetectionByAffiliation.bDetectEnemies = true;
    SightConfig->DetectionByAffiliation.bDetectFriendlies = true;
    SightConfig->DetectionByAffiliation.bDetectNeutrals = true;

    AIPerceptionComponent->ConfigureSense(*SightConfig);
    AIPerceptionComponent->SetDominantSense(SightConfig->GetSenseImplementation());

    AIPerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this, &AEnemyAIController::OnTargetPerceptionUpdated);
}

void AEnemyAIController::BeginPlay()
{
    Super::BeginPlay();

    APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
    AEnemyCharacter* EnemyCharacter = Cast<AEnemyCharacter>(GetPawn());

    if (PlayerCharacter && EnemyCharacter)
    {
        float PlayerSpeed = PlayerCharacter->GetCharacterMovement()->MaxWalkSpeed;
        EnemyCharacter->GetCharacterMovement()->MaxWalkSpeed = PlayerSpeed * 0.6;
    }

    // 스폰된 위치를 저장합니다.
    if (GetPawn())
    {
        InitialSpawnLocation = GetPawn()->GetActorLocation();
    }
}

void AEnemyAIController::OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
    if (Actor->IsA<APlayerCharacter>())
    {
        if (Stimulus.WasSuccessfullySensed())
        {
            StartFollowingPlayer(Actor);
            UpdateAnimInstance(true, 400);  // 이동 중일 때
        }
        else
        {
            StopFollowingPlayer();
        }
    }
}

void AEnemyAIController::MoveToTarget(AActor* TargetActor)
{
    if (TargetActor)
    {
        FVector TargetLocation = TargetActor->GetActorLocation();
        FVector AICharacterLocation = GetPawn()->GetActorLocation();

        float DistanceToTarget = FVector::Dist(AICharacterLocation, TargetLocation);
        float DistanceRadius = 80;

        APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(TargetActor);
        AEnemyCharacter* EnemyCharacter = Cast<AEnemyCharacter>(GetPawn());

        if(PlayerCharacter && EnemyCharacter)
        {
            float PlayerCharacterRadius;
            float PlayerCharacterHeight;
            float EnemyCharacterRadius;
            float EnemyCharacterHeight;
            PlayerCharacter->GetCapsuleComponent()->GetScaledCapsuleSize(PlayerCharacterRadius,PlayerCharacterHeight);
            PlayerCharacter->GetCapsuleComponent()->GetScaledCapsuleSize(EnemyCharacterRadius,EnemyCharacterHeight);
            DistanceRadius = PlayerCharacterRadius + EnemyCharacterRadius;
        }
        
        if (DistanceToTarget > DistanceFromPlayer + DistanceRadius)
        {
            MoveToActor(TargetActor, DistanceFromPlayer, true, true, true, 0, true);
            UpdateAnimInstance(true,400);
        }
        else
        {
            StopMovement();
            UpdateAnimInstance(false, 0);
        }
    }
    else
    {
        UpdateAnimInstance(false, 0);
    }
}

void AEnemyAIController::StartFollowingPlayer(AActor* PlayerActor)
{
    if (!GetWorld()->GetTimerManager().IsTimerActive(MoveToPlayerTimerHandle))
    {
        GetWorld()->GetTimerManager().SetTimer(MoveToPlayerTimerHandle, [this, PlayerActor]() {
            MoveToTarget(PlayerActor);
        }, 0.5f, true);
    }
}

void AEnemyAIController::StopFollowingPlayer()
{
    GetWorld()->GetTimerManager().ClearTimer(MoveToPlayerTimerHandle);
    
    if (GetPawn()->GetActorLocation() != InitialSpawnLocation)
    {
        // 스폰 위치로 이동
        MoveToLocation(InitialSpawnLocation, 5.0f, true, true, true, false);
        UpdateAnimInstance(true, 400);
    }
    else
    {
        UpdateAnimInstance(false,0);
    }
    
}

void AEnemyAIController::UpdateAnimInstance(bool bIsMoving, int32 GroundSpeed)
{
    AEnemyCharacter* EnemyCharacter = Cast<AEnemyCharacter>(GetPawn());
    if (EnemyCharacter)
    {
        UAnimInstance* AnimInstance = EnemyCharacter->GetMesh()->GetAnimInstance();
        if (AnimInstance)
        {
            UPlayerAnimInstance* PlayerAnimInstance = Cast<UPlayerAnimInstance>(AnimInstance);
            if (PlayerAnimInstance)
            {
                PlayerAnimInstance->GroundSpeed = GroundSpeed;
                PlayerAnimInstance->ShouldMove=bIsMoving;
            }
        }
    }
}

void AEnemyAIController::OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result)
{
    Super::OnMoveCompleted(RequestID, Result);

    if (Result.Code == EPathFollowingResult::Success)
    {
        UpdateAnimInstance(false, 0);
    }
}
