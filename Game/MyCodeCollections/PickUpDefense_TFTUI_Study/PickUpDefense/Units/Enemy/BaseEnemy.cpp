// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseEnemy.h"

#include "Components/BoxComponent.h"
#include "PickUpDefense/Stage/SplinePath.h"
#include "PickUpDefense/Managers/CombatEventManager.h"

// Sets default values
ABaseEnemy::ABaseEnemy()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	/* Scene Component */
	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	RootComponent = SceneComponent;
	
	/* Spline Component */
	SplineFollowingComponent = CreateDefaultSubobject<USplineFollowingComponent>(TEXT("SplineFollowingComponent"));
	SplineFollowingComponent->SetupAttachment(SceneComponent);;
	
	if(SplineFollowingComponent)
	{
		SplineFollowingComponent->OnSplineFinished.AddDynamic(this, &ABaseEnemy::OnReachedEnd);
	}

	/* Capsule Collider */
	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	BoxComponent->SetupAttachment(SplineFollowingComponent);

	/* StaticMesh */
	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BaseMesh"));
	BaseMesh->SetupAttachment(BoxComponent);
}

// Called when the game starts or when spawned
void ABaseEnemy::BeginPlay()
{
	Super::BeginPlay();


}

void ABaseEnemy::InitializeEnemy(EEnemyAttributeType const InAttributeType, EEnemyRankType const InRankType)
{
	AttributeType = InAttributeType;

	if(const FBaseEnemyStats* Stats = PackageStats.GetStatData(InAttributeType, InRankType))
	{
		HP = Stats->HP;
		PhysicalDefence = Stats->PhysicalDefence;
		MagicalDefence = Stats->MagicalDefence;
		Speed = Stats->Speed;
		RewardWage = Stats->RewardWage;
		Scale = Stats->MeshScale;
	}

	if(UStaticMesh* MeshData = PackageStats.GetMesh(InAttributeType))
	{
		BaseMesh->SetStaticMesh(MeshData);
		BaseMesh->SetRelativeScale3D(Scale);
		
		/* Set Collider */
		FBoxSphereBounds MeshBounds = BaseMesh->Bounds;
		FVector MeshExtent = MeshBounds.BoxExtent;
		FVector AdjustBoxExtent = MeshExtent * AdjustColliderSize;

		BoxComponent->SetBoxExtent(AdjustBoxExtent);
		BoxComponent->SetRelativeLocation(MeshBounds.Origin);
	}

	SplineFollowingComponent->StartFollowSpline();
}

void ABaseEnemy::Destroyed()
{
	OnTargetEnemyDestroy.Broadcast(this);

	if(UCombatEventManager* CombatEventManager = GetGameInstance()->GetSubsystem<UCombatEventManager>())
	{
		CombatEventManager->OnGlobalEnemyDestroyed.Broadcast(this, bCanReceiveReward, RewardWage);
	}
	
	Super::Destroyed();
}


// Called every frame
void ABaseEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


}

void ABaseEnemy::ApplyDamage(float PhysicalDamage, float MagicalDamage)
{
	float PhysicalDeal = FMath::Max(0.f, (PhysicalDamage - PhysicalDefence));
	float MagicalDeal = FMath::Max(0.f, (MagicalDamage - MagicalDefence));
	
	float TotalDamage = PhysicalDeal + MagicalDeal; 

	
	if(TotalDamage > 0)
	{
		HP -= TotalDamage;

		if(UCombatEventManager* CombatEventManager = GetGameInstance()->GetSubsystem<UCombatEventManager>())
		{
			const int32 OutDamage = FMath::RoundToInt(TotalDamage);
			const int32 OutHP = FMath::RoundToInt(HP);
			
			CombatEventManager->OnGlobalEnemyDamaged.Broadcast(this, OutDamage, OutHP);
		}
		
		if(HP <= 0)
		{
			bCanReceiveReward = true;
			Destroy();
		}
	}
}

void ABaseEnemy::OnReachedEnd()
{
	bCanReceiveReward = false;

	Destroy();
}

void ABaseEnemy::SetSplinePath(const ASplinePath* InSplinePath)
{
	SplineFollowingComponent->SplineComponent = InSplinePath->SplineComponent;

	UE_LOG(LogTemp, Warning, TEXT("Set Spline Path!"));
}



