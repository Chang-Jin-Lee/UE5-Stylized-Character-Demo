// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Perception/AISenseConfig_Sight.h"
#include "ShooterAIController.generated.h"

/**
 * 
 */
UCLASS()
class LENA_API AShooterAIController : public AAIController 
{
	GENERATED_BODY()

public:
	AShooterAIController();
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	bool IsDead() const;
	void StopLogic();
private:
	TObjectPtr<APawn> PlayerPawn;

	// UPROPERTY(EditAnywhere)
	// float AcceptanceRadius = 150;
	
	UPROPERTY(EditAnywhere)
	TObjectPtr<UBehaviorTree> AIBehavior;

	TObjectPtr<UAIPerceptionComponent> AIPerceptionComponent;
	UPROPERTY()
	TObjectPtr<UAISenseConfig_Sight> SightConfig ;
};
