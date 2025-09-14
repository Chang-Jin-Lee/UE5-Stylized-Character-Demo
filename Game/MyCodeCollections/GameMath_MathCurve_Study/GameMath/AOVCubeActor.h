// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AOVCubeActor.generated.h"

UCLASS()
class GAMEMATH_API AAOVCubeActor : public AActor
{
	GENERATED_BODY()

public:
	AAOVCubeActor();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;
	bool IsPlayerInSight();

	UPROPERTY(EditAnywhere)
	TObjectPtr<AActor> Player;
	
	UPROPERTY(EditDefaultsOnly, Category="AOV")
	float AOVAngle = 60;

	UPROPERTY(EditDefaultsOnly, Category="AOV")
	float AOVViewDistance = 800;
};
