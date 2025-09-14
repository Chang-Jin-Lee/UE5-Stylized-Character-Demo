// Fill out your copyright notice in the Description page of Project Settings.


#include "LeibnizGetPI.h"

#include "Kismet/KismetMathLibrary.h"

ULeibnizGetPI::ULeibnizGetPI()
{
	UE_LOG(LogTemp, Warning, TEXT("PI value (UKismetMathLibrary::GetPI()) : %f"), UKismetMathLibrary::GetPI());
	UE_LOG(LogTemp, Warning, TEXT("PI value (LeibnizGetPI) : %f"), CalculatePi(1000));
}

ULeibnizGetPI::~ULeibnizGetPI()
{
}

double ULeibnizGetPI::CalculatePi(int terms)
{
	double pi = 0;
	int sign = 1;

	for (int k = 0; k <= terms; k++) {
		pi += 4.0 * sign / (2 * k + 1);
		sign *= -1;
	}
	return pi;
}
