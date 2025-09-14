// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "Lena_Room02HUD.generated.h"

/**
 * 
 */
UCLASS()
class LENA_API ALena_Room02HUD : public AHUD
{
	GENERATED_BODY()
public:
	virtual void BeginPlay() override;
};
