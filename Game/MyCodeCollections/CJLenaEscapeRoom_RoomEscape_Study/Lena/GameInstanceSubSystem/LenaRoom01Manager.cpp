// Fill out your copyright notice in the Description page of Project Settings.


#include "LenaRoom01Manager.h"

#include "Lena/GameInstance/LenaGameInstance.h"

void ULenaRoom01Manager::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	if(g_LenaGameInstanceIsValid)
	{
		LenaRoom01Data = g_LenaGameInstance->LenaRoom01Data;
		LenaRoom01ItemSpawnData = g_LenaGameInstance->LenaRoom01ItemSpawnData;
		LenaRoom01InteractConditionData = g_LenaGameInstance->LenaRoom01InteractConditionData;
		LenaRoom01UIGuideData = g_LenaGameInstance->LenaRoom01UIGuideData;
	}
}

void ULenaRoom01Manager::Deinitialize()
{
	Super::Deinitialize();
}

void ULenaRoom01Manager::Load()
{
	
}
