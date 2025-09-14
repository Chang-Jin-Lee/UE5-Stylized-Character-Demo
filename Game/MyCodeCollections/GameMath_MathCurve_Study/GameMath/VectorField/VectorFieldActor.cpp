#include "VectorFieldActor.h"
#include "Kismet/GameplayStatics.h"

AVectorFieldActor::AVectorFieldActor()
{
	PrimaryActorTick.bCanEverTick = true;

	repelMesh = CreateDefaultSubobject<UStaticMeshComponent>("repelMesh");
	repelMesh->SetupAttachment(RootComponent);
}

void AVectorFieldActor::BeginPlay()
{
	Super::BeginPlay();
}

void AVectorFieldActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	TArray<AActor*> otherActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AVectorFieldActor::StaticClass(), otherActors);

	for (auto otherActor : otherActors)
	{
		if (otherActor == this) continue;

		FVector dir = (otherActor->GetActorLocation() - this->GetActorLocation());
		float dist = dir.Size();
		
		if (dist < 0.1f) continue; // 너무 가까운 경우 계산 생략

		FVector forceDir = dir.GetSafeNormal();

		// 디버깅 선 그리기
		FColor lineColor = FColor::Green;
		if (dist < repelDistance) lineColor = FColor::Red;
		else if (dist > repelDistance) lineColor = FColor::Blue;
		lineColor.A = 0.2f;
		//DrawDebugLine(GetWorld(), GetActorLocation(), otherActor->GetActorLocation(), lineColor, true);
		
		if (dist < repelDistance)	// 너무 가까우면 밀어내기
		{
			float repelStrength = FMath::Min(repelForce / dist, 1000.0f);
			FVector repel = forceDir * repelStrength;
			repelMesh->AddForce(-repel);
		}
		else if (dist > repelDistance) // 너무 멀면 끌어당기기 
		{
			FVector attractVector = forceDir * (attractForce / dist);
			repelMesh->AddForce(attractVector);
		}
	}
}

