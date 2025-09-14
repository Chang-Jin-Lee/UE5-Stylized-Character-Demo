// Fill out your copyright notice in the Description page of Project Settings.

#include "LenaGameInstance.h"

void ULenaGameInstance::Init()
{
	Super::Init();
}

void ULenaGameInstance::Shutdown()
{
	Super::Shutdown();
}

void ULenaGameInstance::OnWorldChanged(UWorld* OldWorld, UWorld* NewWorld)
{
	Super::OnWorldChanged(OldWorld, NewWorld);
}

TObjectPtr<UCJDataTableMgr> ULenaGameInstance::GetDataTableMgr()
{
	return Super::GetDataTableMgr();
}
