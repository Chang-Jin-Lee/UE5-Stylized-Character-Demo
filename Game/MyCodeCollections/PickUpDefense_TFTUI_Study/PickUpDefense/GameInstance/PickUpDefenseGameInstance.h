// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CoreFramework/GameInstance/CJGameInstance.h"
#include "CoreFramework/GlobalInterface/CJGlobalBase.h"
#include "PickUpDefenseGameInstance.generated.h"

UCLASS()
class PICKUPDEFENSE_API UPickUpDefenseGameInstance : public UCJGameInstance
{
	GENERATED_BODY()

public:
	virtual void Init();
	virtual void Shutdown();
	virtual void OnWorldChanged(UWorld* OldWorld, UWorld* NewWorld) override;
	virtual UCJDataTableMgr* GetDataTableMgr() override;
	
};
#define g_PickupDefenseGameInstance (Cast<UPickUpDefenseGameInstance>(GetWorld()->GetGameInstance()))
#define g_PickupDefenseGameInstanceIsValid (g_PickupDefenseGameInstance != nullptr)