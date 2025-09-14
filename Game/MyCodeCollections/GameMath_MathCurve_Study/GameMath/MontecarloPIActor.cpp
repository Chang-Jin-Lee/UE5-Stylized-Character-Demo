#include "MontecarloPIActor.h"

#include "MontecarloPI.h"

AMontecarloPIActor::AMontecarloPIActor()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AMontecarloPIActor::BeginPlay()
{
	Super::BeginPlay();
}

void AMontecarloPIActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AMontecarloPIActor::NotifyHit(class UPrimitiveComponent* MyComp, AActor* Other,
	class UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal,
	FVector NormalImpulse, const FHitResult& Hit)
{
	Super::NotifyHit(MyComp, Other, OtherComp, bSelfMoved, HitLocation, HitNormal, NormalImpulse, Hit);

	if (Other->GetName() == "Circle")
	{
		UE_LOG(LogTemp, Warning, TEXT("In Circle"));
		if (MontecarloPI)
			MontecarloPI->IncreaseInCircle();
	}
	if (Other->GetName() == "Square")
	{
		UE_LOG(LogTemp, Warning, TEXT("In Square"));
		if (MontecarloPI)
			MontecarloPI->IncreaseInSquare();
	}
}

