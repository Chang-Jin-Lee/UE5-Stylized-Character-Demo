// Fill out your copyright notice in the Description page of Project Settings.


#include "MathBundle.h"

#include "LeibnizGetPI.h"
#include "MontecarloPI.h"
#include "MontecarloPIActor.h"
#include "TaylorSeriesApproximationofSin.h"

AMathBundle::AMathBundle()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AMathBundle::BeginPlay()
{
	Super::BeginPlay();

	ApproximationofSin = NewObject<UTaylorSeriesApproximationofSin>();
	LeibnizGetPI = NewObject<ULeibnizGetPI>();
	MontecarloPI = NewObject<UMontecarloPI>();
}

void AMathBundle::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	CreatedotActor();
}

void AMathBundle::CreatedotActor()
{
	FVector dotPosition = FVector(FMath::RandRange(-500.0f, 500.0f), FMath::RandRange(-500.0f, 500.0f), 50.0f);
	AActor* SpawnedActor = GetWorld()->SpawnActor(dotActorClass, &dotPosition);
	if (AMontecarloPIActor* monteActor = Cast<AMontecarloPIActor>(SpawnedActor))
	{
		monteActor->MontecarloPI = MontecarloPI;
	}
	SpawnedActor->SetLifeSpan(5.0f);
	MontecarloPI->IncreaseDots();
	MontecarloPI->ShowValues();
}