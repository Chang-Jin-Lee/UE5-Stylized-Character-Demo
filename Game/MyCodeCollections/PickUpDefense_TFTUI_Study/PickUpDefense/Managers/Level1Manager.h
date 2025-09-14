// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Tickable.h"
#include "CJLibrary/CoreFramework/GameInstanceSubsystem/CJGameInstanceSubsystem.h"
#include "CoreFramework/GameManager/CJGameManagerBase.h"
#include "CoreFramework/GlobalInterface/CJGlobalBase.h"
#include "Level1Manager.generated.h"

UCLASS()
class PICKUPDEFENSE_API ULevel1Manager : public UCJGameManagerBase
{
	GENERATED_BODY()

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	void test();
};

#define g_Level1Manager (  GetWorld()->GetGameInstance()->GetSubsystem<ULevel1Manager>() )
#define g_Level1ManagerIsValid ( GetWorld()->GetGameInstance()->GetSubsystem<ULevel1Manager>() != nullptr )