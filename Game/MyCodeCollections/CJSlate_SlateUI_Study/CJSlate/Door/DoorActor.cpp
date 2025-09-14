// Fill out your copyright notice in the Description page of Project Settings.


#include "DoorActor.h"


ADoorActor::ADoorActor()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	SetReplicateMovement(true);

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));

	MeshComp->SetupAttachment(SceneComponent);
	SceneComponent->SetupAttachment(GetRootComponent());

	MoveData.EventTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("EventTimeline"));
	IsOpened = false;
}

void ADoorActor::BeginPlay()
{
	Super::BeginPlay();
	if (MoveData.EventCurve)
	{
		MoveData.EventTimelineCallback.BindUFunction(this, FName("HandleEventProgress"));
		MoveData.EventTimeline->AddInterpFloat(MoveData.EventCurve, MoveData.EventTimelineCallback);
	}
	MoveData.InitialLocation = GetActorLocation();
}

void ADoorActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ADoorActor::HandleEventProgress(float Value)
{
	FVector NewLocation = FMath::Lerp(MoveData.InitialLocation, MoveData.TargetLocation, Value);
	SetActorLocation(NewLocation);
}

void ADoorActor::OpenDoor()
{
	MoveData.EventTimeline->PlayFromStart();
	IsOpened = true;
}