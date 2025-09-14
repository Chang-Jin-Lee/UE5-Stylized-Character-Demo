#include "GameMathCubeActor.h"

#include <string>

#include "Components/BoxComponent.h"

AGameMathCubeActor::AGameMathCubeActor()
{
	PrimaryActorTick.bCanEverTick = true;

	Boxcomp = CreateDefaultSubobject<UBoxComponent>(TEXT("Boxcomp"));
	
	Boxcomp->SetupAttachment(RootComponent);
}

void AGameMathCubeActor::BeginPlay()
{
	Super::BeginPlay();
	
	Boxcomp->OnComponentBeginOverlap.AddDynamic(this, &AGameMathCubeActor::OnBoxOverlap);

	moveDir = FVector(
	FMath::RandRange(-1.0f,1.0f),
	FMath::RandRange(-1.0f,1.0f),
	0
	);
}

void AGameMathCubeActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	//BoundCheck();
	
	Move(DeltaTime);
}

void AGameMathCubeActor::Move(float DeltaTime)
{
	FVector cur = GetActorLocation();
	SetActorLocation(cur + moveDir * moveSpeed * DeltaTime);
}

void AGameMathCubeActor::BoundCheck()
{
	FVector cur = GetActorLocation();
	if (cur.X  < -500 || cur.X > 500 || cur.Z < -500 || cur.Z > 500)
	{
		moveSpeed = - moveSpeed;
	}
}

FVector AGameMathCubeActor::ReflectVector(FVector dir, FVector inNormal)
{
	float k = dir.Dot(inNormal);
	return dir -  2 * k * inNormal;
}

void AGameMathCubeActor::OnBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
	bool bFromSweep, const FHitResult& SweepResult)
{
	FVector contactPos = OtherComp->GetComponentLocation();
	FVector normal = OtherComp->GetForwardVector().GetSafeNormal();
	moveDir = ReflectVector(moveDir, normal); // Custom Reflrectio

	DrawDebugLine(GetWorld(), contactPos, 2*normal, FColor::Blue, false, 5);
	DrawDebugLine(GetWorld(), contactPos, 2*moveDir, FColor::Red, false, 5);
	
	GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, "AGameMathCubeActor::OnBoxOverlap");
}