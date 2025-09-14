// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "AnimData.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct FAnimData : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAnimSequence* WalkAnim;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAnimSequence* RunAnim;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAnimSequence* IdleAnim;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<UAnimSequence*> AttackAnims;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<UAnimSequence*> AttackAnimCriticals;
};