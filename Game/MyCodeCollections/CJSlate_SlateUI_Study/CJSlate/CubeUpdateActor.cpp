// Fill out your copyright notice in the Description page of Project Settings.


#include "CubeUpdateActor.h"


// Sets default values
ACubeUpdateActor::ACubeUpdateActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bReplicates = true;                 // 네트워크에서 복제 활성화
	SetReplicateMovement(true);         // 위치 자동 동기화

	CubeMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CubeMesh"));
	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	CubeMesh->SetupAttachment(SceneComponent);
	SceneComponent->SetupAttachment(GetRootComponent());
}

// Called when the game starts or when spawned
void ACubeUpdateActor::BeginPlay()
{
	Super::BeginPlay();

	CubeMesh->OnComponentHit.AddDynamic(this, &ACubeUpdateActor::OnHit);
}

// Called every frame
void ACubeUpdateActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ACubeUpdateActor::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	FVector NormalImpulse, const FHitResult& Hit)
{
	if (HasAuthority()) // 서버라면 직접 이동
	{
		FVector NewLocation = GetActorLocation() + FVector(50, 0, 0);
		SetActorLocation(NewLocation);
		Multicast_MoveCube(NewLocation); // 서버에서 모든 클라이언트로 전파
	}
	else // 클라이언트라면 서버에 이동 요청
	{
		Server_MoveCube(GetActorLocation() + FVector(50, 0, 0));
	}
}

// HasAuthority로 클라, 서버를 나누기
void ACubeUpdateActor::Multicast_MoveCube_Implementation(FVector NewLocation)
{
	if (!HasAuthority()) // 서버가 아닌 경우에만 위치 업데이트
	{
		SetActorLocation(NewLocation);
	}
}

// 서버에서 실행할 함수
void ACubeUpdateActor::Server_MoveCube_Implementation(FVector NewLocation)
{
	SetActorLocation(NewLocation);
	Multicast_MoveCube(NewLocation);
}

// 서버가 유효한지 판단
bool ACubeUpdateActor::Server_MoveCube_Validate(FVector NewLocation)
{
	return true;
}

