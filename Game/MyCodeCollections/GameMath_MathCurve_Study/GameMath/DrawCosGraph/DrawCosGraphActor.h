// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Kismet/KismetMathLibrary.h"
#include "DrawCosGraphActor.generated.h"

UCLASS()
class GAMEMATH_API ADrawCosGraphActor : public AActor
{
	GENERATED_BODY()

public:
	ADrawCosGraphActor();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;
	
	int resolution = 100; // 궤적의 해상도 설정
	float scale = 200.0f; // 궤적의 크기(높이) 설정
	float lineWidth = 0.1f; // 라인의 굵기 설정
	FColor lineColor = FColor::White; // 라인의 색 설정
	float frequency = 1.0f;
	float xRange = 2.0f * UKismetMathLibrary::GetPI();
	
	void DrawCosGraph();
};
