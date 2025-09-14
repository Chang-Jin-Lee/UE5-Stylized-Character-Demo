// Fill out your copyright notice in the Description page of Project Settings.


#include "TaylorSeriesApproximationofSin.h"

#include "Kismet/KismetMathLibrary.h"

UTaylorSeriesApproximationofSin::UTaylorSeriesApproximationofSin()
{
	float xDegrees = 45.0f;
	float xRadians = xDegrees * UKismetMathLibrary::GetPI() / 180.0f;
	float result = ApproximateSin(xRadians, maxTermCount);

	UE_LOG(LogTemp, Warning, TEXT("Approximated sin(%f) = (%f)"), xDegrees, result);
	UE_LOG(LogTemp, Warning, TEXT("FMath::Sin(%f) = (%f)"), xDegrees, xRadians);
}

UTaylorSeriesApproximationofSin::~UTaylorSeriesApproximationofSin()
{
}

float UTaylorSeriesApproximationofSin::ApproximateSin(float x, int MaxTermCount)
{
	float result = 0.0f;
	float numerator = x;			  // x^(2n+1), 처음은 x^1
	float denominator = 1.0f;         // (2n+1)!
	int sign = 1;

	for (int n = 0; n < MaxTermCount; n++)
	{
		result += sign * (numerator / denominator);

		// 다음 항을 위한 업데이트
		int power = 2 * n + 3;      // 다음 항의 지수
		numerator *= x * x;
		denominator *= (2 * n + 2) * (2 * n + 3);
		sign *= -1;
	}

	return result;
}
