// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnemyStats.h"
#include "PickUpDefense/Units/Components/SplineFollowingComponent.h"
#include "GameFramework/Actor.h"
#include "BaseEnemy.generated.h"

class USplineComponent;
class UBoxComponent;
class UCapsuleComponent;
struct FEnemyStats;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTargetEnemyDestroy, const ABaseEnemy*, OnEnemyDestroy);



enum class EEnemyAttributeType : uint8;

UCLASS()
class PICKUPDEFENSE_API ABaseEnemy : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABaseEnemy();

	void InitializeEnemy(EEnemyAttributeType const InAttributeType, EEnemyRankType const InRankType);
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void Destroyed() override;

	/* 적 스탯 */
	FEnemyStats PackageStats;
	
	/* 적 타입 */
	EEnemyAttributeType AttributeType;

	UPROPERTY(EditAnywhere)
	float HP;
	
	UPROPERTY(EditAnywhere)
	float PhysicalDefence;

	UPROPERTY(EditAnywhere)
	float MagicalDefence;
	
	UPROPERTY(EditAnywhere)
	float Speed;

	UPROPERTY(EditAnywhere)
	int32 RewardWage;
	
	UPROPERTY(EditAnywhere)
	FVector Scale;

	UPROPERTY()
	USceneComponent* SceneComponent;

	
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* BaseMesh;
	
	UPROPERTY(EditAnywhere)
	UBoxComponent* BoxComponent;

	UPROPERTY()
	USplineFollowingComponent* SplineFollowingComponent;

	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	virtual void SetEnemyMesh(EEnemyAttributeType const InDeliveryType) PURE_VIRTUAL(ABasePackage::SetPackageMesh, );
	virtual void SetEnemyStats(EEnemyRankType const InPackageType) PURE_VIRTUAL(ABasePackage::SetPackageStats, );

	
	FOnTargetEnemyDestroy OnTargetEnemyDestroy;	

	void ApplyDamage(float PhysicalDamage, float MagicalDamage);

	EEnemyAttributeType GetAttributeType() const { return AttributeType; }

	UPROPERTY(EditAnywhere)
	float AdjustColliderSize = 1.1f;


	bool bCanReceiveReward;

	UFUNCTION()
	void OnReachedEnd();

	UFUNCTION()
	void SetSplinePath(const ASplinePath* InSplinePath);
	
};
