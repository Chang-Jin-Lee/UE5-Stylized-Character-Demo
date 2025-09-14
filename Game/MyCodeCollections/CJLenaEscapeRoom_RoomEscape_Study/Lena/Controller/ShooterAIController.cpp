// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterAIController.h"

#include "BehaviorTree/BehaviorTreeComponent.h"
#include "Lena/Characters/Base_Character.h"
#include "Kismet/GameplayStatics.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "Perception/AIPerceptionComponent.h"

AShooterAIController::AShooterAIController()
{
	AIPerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerceptionComponent"));
	SetPerceptionComponent(*AIPerceptionComponent);

	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SightConfig"));
	SightConfig->SightRadius = 1000.0f;
	SightConfig->LoseSightRadius = 1200.0f;
	SightConfig->PeripheralVisionAngleDegrees = 90.0f;
	SightConfig->SetMaxAge(5.0f);

	SightConfig->DetectionByAffiliation.bDetectEnemies = true;
	SightConfig->DetectionByAffiliation.bDetectNeutrals = true;
	SightConfig->DetectionByAffiliation.bDetectFriendlies = false;

	AIPerceptionComponent->ConfigureSense(*SightConfig);
	AIPerceptionComponent->SetDominantSense(SightConfig->GetSenseImplementation());
}

void AShooterAIController::BeginPlay()
{
	Super::BeginPlay();
	// PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	// SetFocus(PlayerPawn);

	if(AIBehavior)
	{
		RunBehaviorTree(AIBehavior);
		PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
		GetBlackboardComponent()->SetValueAsVector(TEXT("StartLocation"), GetPawn()->GetActorLocation() );
	}
}

void AShooterAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

bool AShooterAIController::IsDead() const
{
	if(ABase_Character* ControlledCharactor = Cast<ABase_Character>(GetPawn()))
		return ControlledCharactor->IsDead();
	return true;
}

void AShooterAIController::StopLogic()
{
	if (UBehaviorTreeComponent* BehaviorTreeComp = Cast<UBehaviorTreeComponent>(GetBrainComponent()))
		BehaviorTreeComp->StopTree(EBTStopMode::Safe);
}
