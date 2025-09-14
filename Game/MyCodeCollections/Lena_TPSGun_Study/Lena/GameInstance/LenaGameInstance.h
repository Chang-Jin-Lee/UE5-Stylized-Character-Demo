// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CoreFramework/GameInstance/CJGameInstance.h"
#include "LenaGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class LENA_API ULenaGameInstance : public UCJGameInstance
{
	GENERATED_BODY()
protected:
	virtual void Init() override;
	virtual void Shutdown() override;
	virtual void OnWorldChanged(UWorld* OldWorld, UWorld* NewWorld) override;
	virtual TObjectPtr<UCJDataTableMgr> GetDataTableMgr() override;
};
#define g_LenaGameInstance (Cast<ULenaGameInstance>(GetWorld()->GetGameInstance()))
#define g_LenaGameInstanceIsValid (g_LenaGameInstance != nullptr)