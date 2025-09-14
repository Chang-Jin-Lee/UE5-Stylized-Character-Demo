// Fill out your copyright notice in the Description page of Project Settings.


#include "StoneActor.h"

#include "CJSlate/Managers/AnswerManager.h"
#include "CJSlate/UI/ButtonWidget.h"
#include "Components/WidgetComponent.h"

class UButtonWidget;

AStoneActor::AStoneActor()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	SetReplicateMovement(true);
	
	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	WidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("WidgetComponent"));

	MeshComp->SetupAttachment(SceneComponent);
	SceneComponent->SetupAttachment(GetRootComponent());
	WidgetComponent->SetupAttachment(SceneComponent);

	MoveData.EventTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("EventTimeline"));
	IsMoved = false;
}

void AStoneActor::BeginPlay()
{
	Super::BeginPlay();
	
	if (MoveData.EventCurve)
	{
		MoveData.EventTimelineCallback.BindUFunction(this, FName("HandleEventProgress"));
		MoveData.EventTimeline->AddInterpFloat(MoveData.EventCurve, MoveData.EventTimelineCallback);
	}
	MoveData.InitialLocation = MeshComp->GetRelativeLocation();
	
	if (WidgetComponent)
	{
		if (UButtonWidget* widget = Cast<UButtonWidget>(WidgetComponent->GetWidget()))
		{
			widget->SetTextBlock(FString::FromInt(StoneID+1));
		}
	}
}

void AStoneActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AStoneActor::HandleEventProgress(float Value)
{
	FVector NewLocation = FMath::Lerp(MoveData.InitialLocation, MoveData.TargetLocation, Value);
	MeshComp->SetRelativeLocation(NewLocation);
}

void AStoneActor::Server_Move_Implementation()
{
	if (IsMoved)
	{
		MoveData.EventTimeline->ReverseFromEnd();
		AnswerManager->BitmaskAnswer(StoneID, false);
		IsMoved = false;
	}
	else
	{
		MoveData.EventTimeline->PlayFromStart();
		AnswerManager->BitmaskAnswer(StoneID, true);
		IsMoved = true;
	}
	MultiCast_Move();
}

bool AStoneActor::Server_Move_Validate()
{
	return true;	
}

void AStoneActor::MultiCast_Move_Implementation()
{
	if (!HasAuthority())
	{
		if (IsMoved)
		{
			MoveData.EventTimeline->ReverseFromEnd();
			AnswerManager->BitmaskAnswer(StoneID, false);
			IsMoved = false;
		}
		else
		{
			MoveData.EventTimeline->PlayFromStart();
			AnswerManager->BitmaskAnswer(StoneID, true);
			IsMoved = true;
		}
	}
}

bool AStoneActor::MultiCast_Move_Validate()
{
	return true;
}

void AStoneActor::Move()
{
	if (HasAuthority())
	{
		if (IsMoved)
		{
			MoveData.EventTimeline->ReverseFromEnd();
			AnswerManager->BitmaskAnswer(StoneID, false);
			IsMoved = false;
		}
		else
		{
			MoveData.EventTimeline->PlayFromStart();
			AnswerManager->BitmaskAnswer(StoneID, true);
			IsMoved = true;
		}
		MultiCast_Move();
	}
	else
	{
		Server_Move();
	}
}
