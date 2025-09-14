// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/TimelineComponent.h"
#include "UObject/Interface.h"

#include "MoveInterface.generated.h"

USTRUCT(BlueprintType)
struct FMoveData
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector InitialLocation;
    
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector TargetLocation;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TObjectPtr<UTimelineComponent> EventTimeline;

	UPROPERTY(EditAnywhere, Category = "Timeline")
	TObjectPtr<UCurveFloat> EventCurve;

	UPROPERTY()
	FOnTimelineFloat EventTimelineCallback;
};

UINTERFACE(MinimalAPI)
class UMoveInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class CJSLATE_API IMoveInterface
{
	GENERATED_BODY()

public:
	virtual void HandleEventProgress(float Value) = 0;
	virtual const FMoveData& GetMoveData() const = 0;
};
