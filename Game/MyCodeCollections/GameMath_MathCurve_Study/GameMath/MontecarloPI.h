// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ConstraintsManager.h"
#include "UObject/Object.h"
#include "MontecarloPI.generated.h"

/**
 * 
 */
UCLASS()
class GAMEMATH_API UMontecarloPI : public UObject
{
	GENERATED_BODY()
public:
	UMontecarloPI();
	~UMontecarloPI();

	UPROPERTY(EditAnywhere)
	FString dotText;
	UPROPERTY(EditAnywhere)
	FString circleText;
	UPROPERTY(EditAnywhere)
	FString squareText;
	UPROPERTY(EditAnywhere)
	FString piText;

	int numDots;
	int numInCircle;
	int numInSquare;
	float piEstimate;

	UFUNCTION()
	void ShowValues();
	
	void IncreaseDots();
	void IncreaseInCircle();
	void IncreaseInSquare();
	void CalculatePiEst();
};
