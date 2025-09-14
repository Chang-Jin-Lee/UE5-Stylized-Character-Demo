// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MathBundle.generated.h"

class UMontecarloPI;
class ULeibnizGetPI;
class UTaylorSeriesApproximationofSin;

UCLASS()
class GAMEMATH_API AMathBundle : public AActor
{
	GENERATED_BODY()

public:
	AMathBundle();

protected:
	virtual void BeginPlay() override;
	
public:
	virtual void Tick(float DeltaTime) override;

public:
	UTaylorSeriesApproximationofSin* ApproximationofSin;
	ULeibnizGetPI* LeibnizGetPI;
	UMontecarloPI* MontecarloPI;

	UPROPERTY(EditAnywhere)
	TSubclassOf<AActor> dotActorClass;

	void CreatedotActor();
};
