// Fill out your copyright notice in the Description page of Project Settings.


#include "AOVCubeActor.h"

AAOVCubeActor::AAOVCubeActor()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AAOVCubeActor::BeginPlay()
{
	Super::BeginPlay();
	
}

void AAOVCubeActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (IsPlayerInSight() == false)
	{
		DrawDebugCone(GetWorld(), GetActorLocation(), GetActorForwardVector(), AOVViewDistance, FMath::DegreesToRadians(AOVAngle), 90 - FMath::DegreesToRadians(AOVAngle), 50, FColor::Green, false, 5);
	}
}

bool AAOVCubeActor::IsPlayerInSight()
{
	if (Player)
	{
		FVector dir = Player->GetActorLocation() - GetActorLocation();
		float distanceToPlayer = dir.Length();

		if (distanceToPlayer < AOVViewDistance)
			return false;

		dir.Normalize();
		float dot = GetActorForwardVector().Dot(dir);
		const float cosThreshold = FMath::Cos(AOVAngle * 0.5f);

		if (dot < cosThreshold)
			return false;

		FHitResult hit;
		if (GetWorld()->LineTraceSingleByChannel(hit, GetActorLocation(), GetActorLocation() + dir * AOVViewDistance, ECC_Visibility))
		{
			DrawDebugLine(GetWorld(), GetActorLocation(), GetActorLocation() + dir * AOVViewDistance,  FColor::Red, false, 5);
			return true;
		}
	}
	return false;
}