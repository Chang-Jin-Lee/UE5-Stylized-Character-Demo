// Fill out your copyright notice in the Description page of Project Settings.


#include "MontecarloPI.h"

#include "MontecarloPIActor.h"

UMontecarloPI::UMontecarloPI()
{
}

UMontecarloPI::~UMontecarloPI()
{
}

void UMontecarloPI::ShowValues()
{
	dotText = FString::Printf(TEXT("(Dots) %d") , numDots);
	circleText = FString::Printf(TEXT("(in Circle) %d") , numInCircle);
	squareText = FString::Printf(TEXT("(in Square) %d") , numInSquare);
	piText = FString::Printf(TEXT("(PI(Est.)) %f F3") , piEstimate);
}

void UMontecarloPI::IncreaseDots()
{
	numDots++;
}

void UMontecarloPI::IncreaseInCircle()
{
	numInCircle++;
	CalculatePiEst();
}

void UMontecarloPI::IncreaseInSquare()
{
	numInSquare++;
	CalculatePiEst();
}

void UMontecarloPI::CalculatePiEst()
{
	if(numInSquare > 0)
	{
		piEstimate = (float) (numInCircle * 4.0f) / (float) numInSquare;
	}
}
