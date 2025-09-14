// Fill out your copyright notice in the Description page of Project Settings.


#include "SplineFollowingComponent.h"
#include "Components/SplineComponent.h"

// Sets default values for this component's properties
USplineFollowingComponent::USplineFollowingComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void USplineFollowingComponent::BeginPlay()
{
	Super::BeginPlay();

	MovementCurve = NewObject<UCurveFloat>();

	FKeyHandle Key1 = MovementCurve->FloatCurve.AddKey(0.f, 0.f);
	FKeyHandle Key2 = MovementCurve->FloatCurve.AddKey(MovementDuration, 1.f);

	MovementCurve->FloatCurve.SetKeyInterpMode(Key1, RCIM_Linear);
	MovementCurve->FloatCurve.SetKeyInterpMode(Key2, RCIM_Linear);
	
	
	if(MovementCurve)
	{
		FOnTimelineFloat TimelineFloat;
		TimelineFloat.BindUFunction(this, FName("UpdatePath"));
		MovementTimeline.AddInterpFloat(MovementCurve, TimelineFloat);

		FOnTimelineEvent TimelineFinishedEvent;
		TimelineFinishedEvent.BindUFunction(this, FName("OnFinished"));
		MovementTimeline.SetTimelineFinishedFunc(TimelineFinishedEvent);

		MovementTimeline.SetPlayRate(Speed);
	}
}


// Called every frame
void USplineFollowingComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if(MovementTimeline.IsPlaying())
	{
		MovementTimeline.TickTimeline(DeltaTime);
	}
}

void USplineFollowingComponent::StartFollowSpline()
{
	if(SplineComponent && MovementCurve)
	{
		MovementTimeline.PlayFromStart();
	}
}

void USplineFollowingComponent::UpdatePath(float value)
{
	if(SplineComponent)
	{
		float SplineLength = SplineComponent->GetSplineLength();
		FVector NewLocation = SplineComponent->GetLocationAtDistanceAlongSpline(SplineLength * value, ESplineCoordinateSpace::World);
		FRotator NewRotation = SplineComponent->GetRotationAtDistanceAlongSpline(SplineLength * value, ESplineCoordinateSpace::World);

		AActor* Owner = GetOwner();
		if(Owner)
		{
			Owner->SetActorLocation(NewLocation);
			Owner->SetActorRotation(NewRotation);
		}
	}
}

void USplineFollowingComponent::OnFinished()
{
	OnSplineFinished.Broadcast();
}

void USplineFollowingComponent::SetMovementSpeed()
{
	
}

