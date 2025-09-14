// Fill out your copyright notice in the Description page of Project Settings.


#include "LenaRoom02Manager.h"

#include "Lena/GameInstance/LenaGameInstance.h"

void ULenaRoom02Manager::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	if(g_LenaGameInstanceIsValid)
	{
		LenaRoom02Data = g_LenaGameInstance->LenaRoom02Data;
		LenaRoom02InteractConditionData = g_LenaGameInstance->LenaRoom02InteractConditionData;
	}
}

void ULenaRoom02Manager::Deinitialize()
{
	Super::Deinitialize();
}
