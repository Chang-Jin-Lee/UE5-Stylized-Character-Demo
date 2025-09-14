// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "LeibnizGetPI.generated.h"

/**
 * 
 */
UCLASS()
class GAMEMATH_API ULeibnizGetPI : public UObject
{
	GENERATED_BODY()

public:
	ULeibnizGetPI();
	~ULeibnizGetPI();

	double CalculatePi(int terms);
};
