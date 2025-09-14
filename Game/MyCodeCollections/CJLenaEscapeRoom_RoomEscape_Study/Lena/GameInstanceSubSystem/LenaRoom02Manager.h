// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CoreFramework/GameManager/CJGameManagerBase.h"
#include "CJLibrary/ManagerRegistry/Data/LenaRoom02JsonData.h"
#include "CoreFramework/GlobalInterface/CJGlobalBase.h"
#include "LenaRoom02Manager.generated.h"

/**
 * 
 */
UCLASS()
class LENA_API ULenaRoom02Manager : public UCJGameManagerBase
{
	GENERATED_BODY()
	
public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

	FORCEINLINE TArray<FLenaRoom02DataRow> GetLenaRoom02Data() { return LenaRoom02Data;}
	FORCEINLINE TArray<FLenaRoom02InteractConditionRow> GetLenaRoom02InteractConditionData() { return LenaRoom02InteractConditionData;}

private:
	TArray<FLenaRoom02DataRow> LenaRoom02Data;
	TArray<FLenaRoom02InteractConditionRow> LenaRoom02InteractConditionData;
};
#define g_LenaRoom02Manager (  GetWorld()->GetGameInstance()->GetSubsystem<ULenaRoom02Manager>() )
#define g_LenaRoom02ManagerIsValid ( GetWorld()->GetGameInstance()->GetSubsystem<ULenaRoom02Manager>() != nullptr )
