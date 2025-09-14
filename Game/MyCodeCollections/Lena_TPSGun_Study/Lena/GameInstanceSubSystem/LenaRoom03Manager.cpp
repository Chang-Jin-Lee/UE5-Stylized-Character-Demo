// Fill out your copyright notice in the Description page of Project Settings.


#include "LenaRoom03Manager.h"

#include "Lena/GameInstance/LenaGameInstance.h"

void ULenaRoom03Manager::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	if(g_LenaGameInstanceIsValid)
	{
		LenaRoom03Data = g_LenaGameInstance->LenaRoom03Data;
		LenaRoom03ItemSpawnData = g_LenaGameInstance->LenaRoom03ItemSpawnData;
		LenaRoom03UIGuideData = g_LenaGameInstance->LenaRoom03UIGuideData;
	}
}

void ULenaRoom03Manager::Deinitialize()
{
	Super::Deinitialize();
}