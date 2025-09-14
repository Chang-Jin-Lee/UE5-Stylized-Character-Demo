// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Base_Item.h"
#include "Base_Weapon.generated.h"

UCLASS()
class LENA_API ABase_Weapon : public ABase_Item
{
	GENERATED_BODY()

public:
	ABase_Weapon();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;
};
