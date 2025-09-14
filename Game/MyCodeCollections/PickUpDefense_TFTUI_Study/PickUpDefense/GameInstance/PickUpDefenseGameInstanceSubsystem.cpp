// Fill out your copyright notice in the Description page of Project Settings.


#include "PickUpDefenseGameInstanceSubsystem.h"

void UPickUpDefenseGameInstanceSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	UE_LOG(LogTemp, Warning, TEXT("UPickUpDefenseGameInstanceSubsystem Initialize"));
	Super::Initialize(Collection);
}

void UPickUpDefenseGameInstanceSubsystem::test()
{
	UE_LOG(LogTemp, Warning, TEXT("test"));
}
