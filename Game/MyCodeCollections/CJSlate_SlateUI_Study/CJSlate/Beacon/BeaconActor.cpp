// Fill out your copyright notice in the Description page of Project Settings.


#include "BeaconActor.h"
#include "CJSlate/Managers/AnswerManager.h"
#include "Components/BoxComponent.h"

ABeaconActor::ABeaconActor()
{
	PrimaryActorTick.bCanEverTick = true;

	bReplicates = true;
	SetReplicateMovement(true);
	
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	CollisionBox->OnComponentBeginOverlap.AddDynamic(this, &ABeaconActor::OnOverlapBegin);

	CollisionBox->SetupAttachment(MeshComp);
	MeshComp->SetupAttachment(SceneComponent);
	SceneComponent->SetupAttachment(GetRootComponent());
}

void ABeaconActor::BeginPlay()
{
	Super::BeginPlay();

	StartingLocation = GetActorLocation();
}

void ABeaconActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABeaconActor::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor == this || OtherComp->GetOwner() == this) return;
	CollisionBox->SetCollisionEnabled(ECollisionEnabled::Type::NoCollision);
	if (HasAuthority()) // 서버라면
	{
		FVector NewLocation = GetActorLocation() + AddLocation;
		SetActorLocation(NewLocation);
		BeaconManager->AddAnswer(BeaconID);
		BeaconManager->IsCorrectAnswer();
		Multicast_Move(NewLocation);
	}
	else // 클라라면
	{
		Server_Move(GetActorLocation() +  AddLocation);
	}
}

void ABeaconActor::Multicast_Move_Implementation(FVector NewLocation)
{
	if (!HasAuthority()) // 클라라면
	{
		BeaconManager->AddAnswer(BeaconID);
		BeaconManager->IsCorrectAnswer();
		SetActorLocation(NewLocation);
	}
}

bool ABeaconActor::Multicast_Move_Validate(FVector NewLocation)
{
	return true;
}

void ABeaconActor::Server_Move_Implementation(FVector NewLocation)
{
	BeaconManager->AddAnswer(BeaconID);
	BeaconManager->IsCorrectAnswer();
	SetActorLocation(NewLocation);
	Multicast_Move(NewLocation);
}

bool ABeaconActor::Server_Move_Validate(FVector NewLocation)
{
	return true;
}

