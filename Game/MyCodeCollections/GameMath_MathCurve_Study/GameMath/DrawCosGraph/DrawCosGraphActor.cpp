// Fill out your copyright notice in the Description page of Project Settings.


#include "DrawCosGraphActor.h"

#include "Engine/StaticMeshActor.h"


ADrawCosGraphActor::ADrawCosGraphActor()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ADrawCosGraphActor::BeginPlay()
{
	Super::BeginPlay();
	DrawCosGraph();
}

void ADrawCosGraphActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ADrawCosGraphActor::DrawCosGraph()
{
	float steps = xRange / resolution;

	for (int i = 0; i < resolution; i++)
	{
		float x = i * steps - xRange / 2.0f; // 가운데 정렬
		float y = FMath::Cos(frequency * x) * scale;

		FVector Location = FVector(x * 100.0f, y, 0);
		FActorSpawnParameters SpawnParams;
		AStaticMeshActor* SphereActor = GetWorld()->SpawnActor<AStaticMeshActor>(
			AStaticMeshActor::StaticClass(), Location, FRotator::ZeroRotator, SpawnParams);
		if (SphereActor)
		{
			UStaticMeshComponent* MeshComp = SphereActor->GetStaticMeshComponent();
			// 기본 구 메쉬 로드
			UStaticMesh* SphereMesh = LoadObject<UStaticMesh>(nullptr, TEXT("/Engine/BasicShapes/Sphere.Sphere"));
			if (SphereMesh)
			{
				MeshComp->SetStaticMesh(SphereMesh);
				MeshComp->SetWorldScale3D(FVector(lineWidth)); // 작게 조정
				MeshComp->SetMobility(EComponentMobility::Movable);
			}
			SphereActor->SetActorEnableCollision(false); // 충돌 비활성화
		}
	}
}
