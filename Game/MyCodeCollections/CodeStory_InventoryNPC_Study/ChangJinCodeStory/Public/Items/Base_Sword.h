// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Base_Weapon.h"
#include "Base_Sword.generated.h"

UCLASS()
class CHANGJINCODESTORY_API ABase_Sword : public ABase_Weapon
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ABase_Sword();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, Category="ItemMesh")
	UStaticMeshComponent* SwordStaticMeshComponent;
};
