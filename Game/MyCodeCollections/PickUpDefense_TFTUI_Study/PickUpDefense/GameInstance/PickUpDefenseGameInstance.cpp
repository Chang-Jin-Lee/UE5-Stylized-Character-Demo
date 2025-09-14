// Fill out your copyright notice in the Description page of Project Settings.


#include "PickUpDefenseGameInstance.h"

#include "DataTable/DataManager/CJDataTableMgr.h"

void UPickUpDefenseGameInstance::Init()
{
	Super::Init();
}

void UPickUpDefenseGameInstance::Shutdown()
{
	Super::Shutdown();
}

void UPickUpDefenseGameInstance::OnWorldChanged(UWorld* OldWorld, UWorld* NewWorld)
{
	Super::OnWorldChanged(OldWorld, NewWorld);
}

UCJDataTableMgr* UPickUpDefenseGameInstance::GetDataTableMgr()
{
	return Super::GetDataTableMgr();
}