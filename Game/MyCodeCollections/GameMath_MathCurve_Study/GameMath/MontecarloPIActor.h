// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MontecarloPIActor.generated.h"

class UMontecarloPI;

UCLASS()
class GAMEMATH_API AMontecarloPIActor : public AActor
{
	GENERATED_BODY()

public:
	AMontecarloPIActor();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;
	virtual void NotifyHit(class UPrimitiveComponent* MyComp, AActor* Other, class UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit) override;

	UMontecarloPI* MontecarloPI;
};
