// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LenaGameMode.h"
#include "GameFramework/GameModeBase.h"
#include "LenaRoom03GameMode.generated.h"

/**
 * 
 */
UCLASS()
class LENA_API ALenaRoom03GameMode : public ALenaGameMode
{
	GENERATED_BODY()

public:
	virtual void PawnKilled(TObjectPtr<APawn> PawnKilled) override;

private:
	void EndGame(bool bIsPlayerWinner);
};
