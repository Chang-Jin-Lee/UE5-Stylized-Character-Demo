// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Lena/Data/ConditionMapping.h"
#include "InteractManager.generated.h"

UCLASS()
class LENA_API AInteractManager : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AInteractManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	void SetupLockAndDoor();
	void SetupConditionWithActor(TObjectPtr<AActor> Actor, const FConditionEntry& ConditionEntry);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(allowPrivateAccess=true))
	TArray<TObjectPtr<AActor>> InteractActors;
	TMap<FString, TObjectPtr<AActor>> LockMap;
	TMap<FString, TObjectPtr<AActor>> DoorMap;
	TMap<FString, TArray<TObjectPtr<AActor>>> ItemMap;
	TMap<FString, TObjectPtr<AActor>> ButtonMap;
};
