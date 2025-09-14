// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"

#include "UIPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class PICKUPDEFENSE_API AUIPlayerController : public APlayerController
{
	GENERATED_BODY()

	AUIPlayerController();
	virtual void BeginPlay() override;
};
