#pragma once

/*
 *	@briefs : VectorFieldActor를 스폰하는 스포너입니다.
 *	@details : 스포너로 스폰하는 이유는 Actor마다 다른 특성을 세팅할 수 있게 하기 위함입니다.
 */

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "VectorFieldActorSpawner.generated.h"

class AVectorFieldActor;

UCLASS()
class GAMEMATH_API AVectorFieldActorSpawner : public AActor
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn Class")
	TSubclassOf<AVectorFieldActor> vectorFieldClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability")
	float spawnInterval = 0.1f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability")
	float timer = 0.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability")
	int currentCircleCount = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability")
	int maxCircleCount = 50;

	FTimerHandle timerHandle;

	UFUNCTION()
	void SpawnActor();

public:
	AVectorFieldActorSpawner();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;
};
