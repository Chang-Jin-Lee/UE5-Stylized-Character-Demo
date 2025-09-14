#include "VectorFieldActorSpawner.h"
#include "VectorFieldActor.h"

void AVectorFieldActorSpawner::SpawnActor()
{
	FVector spawnPos = FMath::VRand() * 100.0f;
	FActorSpawnParameters spawnParams;
	AVectorFieldActor* vectorFieldActor = GetWorld()->SpawnActor<AVectorFieldActor>(vectorFieldClass, spawnPos, FRotator::ZeroRotator, spawnParams);

	currentCircleCount++;
	// 원 색상은 랜덤하게 설정하기
}

AVectorFieldActorSpawner::AVectorFieldActorSpawner()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AVectorFieldActorSpawner::BeginPlay()
{
	Super::BeginPlay();
	GetWorld()->GetTimerManager().ClearTimer(timerHandle);
	GetWorld()->GetTimerManager().SetTimer(timerHandle, this, &AVectorFieldActorSpawner::SpawnActor, spawnInterval, true);
}

void AVectorFieldActorSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (GetWorld()->GetTimerManager().IsTimerActive(timerHandle))
	{
		if (maxCircleCount < currentCircleCount)
		{
			GetWorld()->GetTimerManager().PauseTimer(timerHandle);
		}
	}
}