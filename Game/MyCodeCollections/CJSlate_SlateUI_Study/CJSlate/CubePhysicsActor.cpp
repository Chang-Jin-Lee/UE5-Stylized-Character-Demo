// Fill out your copyright notice in the Description page of Project Settings.


#include "CubePhysicsActor.h"

// Sets default values
ACubePhysicsActor::ACubePhysicsActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	bReplicates = true;                 // 네트워크에서 복제 활성화
	SetReplicateMovement(true);         // 위치 자동 동기화
	SetReplicatingMovement(true);		// 물리 움직임 복제
	
	CubeMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CubeMesh"));
	CubeMesh->SetSimulatePhysics(true); // 물리 엔진 활성화
	CubeMesh->SetEnableGravity(true);   // 중력 적용
	CubeMesh->SetIsReplicated(true);    // 네트워크 복제 설정
	CubeMesh->SetCollisionProfileName(TEXT("PhysicsActor")); // 충돌 설정
	
	CubeMesh->OnComponentHit.AddDynamic(this, &ACubePhysicsActor::OnHit);

	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	CubeMesh->SetupAttachment(SceneComponent);
	SceneComponent->SetupAttachment(GetRootComponent());
	
}

// Called when the game starts or when spawned
void ACubePhysicsActor::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ACubePhysicsActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ACubePhysicsActor::Multicast_UpdatePhysicsState_Implementation(FVector NewVelocity, FVector NewAngularVelocity)
{
	if (CubeMesh && !HasAuthority()) // 서버가 아닌 경우만 적용
	{
		CubeMesh->SetPhysicsLinearVelocity(NewVelocity);
		CubeMesh->SetPhysicsAngularVelocityInDegrees(NewAngularVelocity);
	}
}

void ACubePhysicsActor::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
                       FVector NormalImpulse, const FHitResult& Hit)
{
	if (HasAuthority()) // 서버에서만 처리
	{
		if (CubeMesh)
		{
			FVector Velocity = CubeMesh->GetPhysicsLinearVelocity();
			FVector AngularVelocity = CubeMesh->GetPhysicsAngularVelocityInDegrees();

			Multicast_UpdatePhysicsState(Velocity, AngularVelocity);
		}
	}
}

