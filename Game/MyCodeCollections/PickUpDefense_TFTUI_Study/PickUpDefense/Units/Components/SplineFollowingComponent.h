// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "Components/TimelineComponent.h"
#include "SplineFollowingComponent.generated.h"

class USplineComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnSplineFinished);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PICKUPDEFENSE_API USplineFollowingComponent : public USceneComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USplineFollowingComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void StartFollowSpline();

	UPROPERTY()
	FOnSplineFinished OnSplineFinished;
	
	UPROPERTY()
	USplineComponent* SplineComponent = nullptr;

	UPROPERTY()
	UCurveFloat* MovementCurve = nullptr;

	UPROPERTY(EditAnywhere)
	float Speed = 1.f;

	UPROPERTY(EditAnywhere)
	float MovementDuration = 15.f;
	
private:
	FTimeline MovementTimeline;

	UFUNCTION()
	void UpdatePath(float value);

	UFUNCTION()
	void OnFinished();

	UFUNCTION()
	void SetMovementSpeed();
};
