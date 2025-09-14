// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "TaylorSeriesApproximationofSin.generated.h"

/**
 * 
 */
UCLASS()
class GAMEMATH_API UTaylorSeriesApproximationofSin : public UObject
{
	GENERATED_BODY()

public:
	UTaylorSeriesApproximationofSin();
	~UTaylorSeriesApproximationofSin();

	float ApproximateSin(float x, int MaxTermCount);

private:
	int maxTermCount = 10;
};