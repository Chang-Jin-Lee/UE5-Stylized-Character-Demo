// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Lena_IntroGamePlayerController.generated.h"

/**
 * 
 */
UCLASS()
class LENA_API ALena_IntroGamePlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	virtual void BeginPlay() override;
};
