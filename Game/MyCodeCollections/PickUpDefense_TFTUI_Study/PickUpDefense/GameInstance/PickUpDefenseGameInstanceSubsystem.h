// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CJLibrary/CoreFramework/GameInstanceSubsystem/CJGameInstanceSubsystem.h"
#include "PickUpDefenseGameInstanceSubsystem.generated.h"

/**
 * 
 */
UCLASS()
class PICKUPDEFENSE_API UPickUpDefenseGameInstanceSubsystem : public UCJGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	void test();
};