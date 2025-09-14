// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CoreFramework/GameManager/CJGameManagerBase.h"
#include "Lena/Characters/AI_Character.h"
#include "LenaEnemyManager.generated.h"

/**
 * 
 */
UCLASS()
class LENA_API ULenaEnemyManager : public UCJGameManagerBase
{
	GENERATED_BODY()
public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

	FORCEINLINE int32 GetEnemyCount(){return EnemyCount;};
	FORCEINLINE void SetEnemyCount(int32 _in){EnemyCount = _in;}

private:
	int32 EnemyCount = 0;
};
#define g_LenaEnemyManager (  GetWorld()->GetGameInstance()->GetSubsystem<ULenaEnemyManager>() )
#define g_LenaEnemyManagerIsValid ( GetWorld()->GetGameInstance()->GetSubsystem<ULenaEnemyManager>() != nullptr )