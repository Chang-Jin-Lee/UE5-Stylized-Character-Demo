// Fill out your copyright notice in the Description page of Project Settings.


#include "Lena/Actor/InteractableThings/Lock/CombinationLockAlphabetActor.h"
#include "Components/TimelineComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Blueprint/UserWidget.h"
#include "Components/BoxComponent.h"
#include "Lena/Actor/InteractableThings/Door/SlidingDoorActor.h"
#include "Kismet/GameplayStatics.h"
#include "Rendering/RenderCommandPipes.h"
#include "Lena/UI/InteractWidget.h"

// Sets default values
ACombinationLockAlphabetActor::ACombinationLockAlphabetActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	WheelSceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("WheelSceneComponent"));
	WheelSceneComponent->SetupAttachment(RootComponent);

	WheelMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WheelMesh"));
	if(WheelMesh)
	{
		WheelMesh->SetupAttachment(WheelSceneComponent);
		WheelMesh->SetRelativeLocation(FVector(-9,0,0)* WheelMeshDefaultScaleSize);
		WheelMesh->SetRelativeScale3D(FVector::One() * WheelMeshDefaultScaleSize);
	}
	
	WheelMesh1 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WheelMesh1"));
	if(WheelMesh1)
	{
		WheelMesh1->SetupAttachment(WheelSceneComponent);
		WheelMesh1->SetRelativeLocation(FVector(-3,0,0)* WheelMeshDefaultScaleSize);
		WheelMesh1->SetRelativeScale3D(FVector::One() * WheelMeshDefaultScaleSize);
	}

	WheelMesh2 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WheelMesh2"));
	if(WheelMesh2)
	{
		WheelMesh2->SetupAttachment(WheelSceneComponent);
		WheelMesh2->SetRelativeLocation(FVector(3,0,0)* WheelMeshDefaultScaleSize);
		WheelMesh2->SetRelativeScale3D(FVector::One() * WheelMeshDefaultScaleSize);
	}
	

	WheelMesh3 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WheelMesh3"));
	if(WheelMesh3)
	{
		WheelMesh3->SetupAttachment(WheelSceneComponent);
		WheelMesh3->SetRelativeLocation(FVector(9,0,0)* WheelMeshDefaultScaleSize);
		WheelMesh3->SetRelativeScale3D(FVector::One() * WheelMeshDefaultScaleSize);
	}
	

	WheelMesh4 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WheelMesh4"));
	if(WheelMesh4)
	{
		WheelMesh4->SetupAttachment(WheelSceneComponent);
		WheelMesh4->SetRelativeLocation(FVector(15,0,0)* WheelMeshDefaultScaleSize);
		WheelMesh4->SetRelativeScale3D(FVector::One() * WheelMeshDefaultScaleSize);
	}
	

	WheelMesh5 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WheelMesh5"));
	if(WheelMesh5)
	{
		WheelMesh5->SetupAttachment(WheelSceneComponent);
		WheelMesh5->SetRelativeLocation(FVector(-21,0,0)* WheelMeshDefaultScaleSize);
		WheelMesh5->SetRelativeScale3D(FVector::One() * WheelMeshDefaultScaleSize);
	}
	

	WheelMesh6 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WheelMesh6"));
	if(WheelMesh6)
	{
		WheelMesh6->SetupAttachment(WheelSceneComponent);
		WheelMesh6->SetRelativeLocation(FVector(-15,0,0)* WheelMeshDefaultScaleSize);
		WheelMesh6->SetRelativeScale3D(FVector::One() * WheelMeshDefaultScaleSize);
	}
	
	ShackleMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("ShackleMesh"));
	if(ShackleMesh)
	{
		ShackleMesh->SetupAttachment(WheelSceneComponent);
		ShackleMesh->SetRelativeLocation(FVector(-8,0,0)* WheelMeshDefaultScaleSize);
		ShackleMesh->SetRelativeScale3D(FVector::One() * WheelMeshDefaultScaleSize);
	}
	
	CombinationLockTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("CombinationLockTimeline"));
	bIsTimeLinePlaying = false;
	SelectedWheelIndex = 0;
	WheelMeshArray = {WheelMesh5, WheelMesh6, WheelMesh, WheelMesh1, WheelMesh2, WheelMesh3, WheelMesh4};
	// SelectedWheelMeshEnum = EWheelMeshEnum::WheelMesh;
}

// Called when the game starts or when spawned
void ACombinationLockAlphabetActor::BeginPlay()
{
	Super::BeginPlay();

	SetInstructionWidgetText(FText::FromString("Drag Mouse Wheel"), FLinearColor::White);
	SetInstructionWidgetTextAtBeginPlay(FText::FromString("Drag Mouse Wheel"));
	
	if (CombinationLockCurve)
	{
		CombinationLockTimelineCallback.BindUFunction(this, FName("HandleCombinationLockProgress"));
		CombinationLockTimeline->AddInterpFloat(CombinationLockCurve, CombinationLockTimelineCallback);
		TimelineFinishedCallback.BindUFunction(this, FName("OnTimelineFinished"));
		CombinationLockTimeline->SetTimelineFinishedFunc(TimelineFinishedCallback);
	}

	WheelMeshSelectedSize = WheelMeshDefaultScaleSize * 1.1f;
	if(WheelMeshArray.Num() > SelectedWheelIndex)
		WheelMeshArray[SelectedWheelIndex]->SetRelativeScale3D(FVector::One() * WheelMeshSelectedSize);
}

// Called every frame
void ACombinationLockAlphabetActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ACombinationLockAlphabetActor::Unlock(AActor* ActorToUnlock)
{
	Super::Unlock(ActorToUnlock);
	ADoorActor* Door = Cast<ADoorActor>(ActorToUnlock);
	if(Door)
	{
		Door->Open();
	}
}


void ACombinationLockAlphabetActor::ScrollCombinationLock(FRotator InTargetRotation)
{
	if(bIsTimeLinePlaying || !WheelMeshArray[SelectedWheelIndex]) return;
	
	InitialRotation = WheelMeshArray[SelectedWheelIndex]->GetRelativeRotation();
	TargetRotation = InitialRotation + InTargetRotation;
	if (CombinationLockTimeline)
	{
		bIsTimeLinePlaying = true;
		CombinationLockTimeline->PlayFromStart();
	}
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), SoundEffect, GetActorLocation());
}

void ACombinationLockAlphabetActor::HandleCombinationLockProgress(float Value)
{
	if(WheelMeshArray[SelectedWheelIndex])
	{
		FRotator NewRotation = FMath::Lerp(InitialRotation, TargetRotation, Value);
		WheelMeshArray[SelectedWheelIndex]->SetRelativeRotation(NewRotation);
	}
}

void ACombinationLockAlphabetActor::OnTimeLineFinished()
{
	bIsTimeLinePlaying = false;

	CheckRightAnswer();
}

void ACombinationLockAlphabetActor::MoveNextWheelMesh()
{
	if (!bIsTimeLinePlaying && WheelMeshArray.Num() > 0)
	{
		WheelMeshArray[SelectedWheelIndex]->SetRelativeScale3D(FVector::One() * WheelMeshDefaultScaleSize);
		SelectedWheelIndex = (SelectedWheelIndex+1) % WheelMeshArray.Num();
		WheelMeshArray[SelectedWheelIndex]->SetRelativeScale3D(FVector::One() * WheelMeshSelectedSize);
	}
}

void ACombinationLockAlphabetActor::MovePrevWheelMesh()
{
	if (!bIsTimeLinePlaying && WheelMeshArray.Num() > 0)
	{
		WheelMeshArray[SelectedWheelIndex]->SetRelativeScale3D(FVector::One() * WheelMeshDefaultScaleSize);
		SelectedWheelIndex = (SelectedWheelIndex-1 + WheelMeshArray.Num()) % WheelMeshArray.Num();
		WheelMeshArray[SelectedWheelIndex]->SetRelativeScale3D(FVector::One() * WheelMeshSelectedSize);
	}
}

FString ACombinationLockAlphabetActor::GetCurrentDial()
{
	FString RString = "";

	for(UStaticMeshComponent* InMesh : WheelMeshArray)
	{
		RString += GetCurrentDial(InMesh);
	}

	UE_LOG(LogTemp, Warning, TEXT("RString :  %s"), *RString);
	
	return RString;
}


FString ACombinationLockAlphabetActor::GetCurrentDial(UStaticMeshComponent* WheelMeshDial)
{

	// 현재 Roll 값을 가져옴
	float RollValue = WheelMeshDial->GetRelativeRotation().Roll;
	// Roll 값을 -180에서 180 범위로 정규화
	RollValue = FMath::Fmod(RollValue + 180.0f, 360.0f) - 180.0f;

	// 음수 값을 양수 값으로 변환
	if (RollValue < 0)
	{
		RollValue += 360.0f;
	}

	int PositionIndex = FMath::RoundToInt(RollValue / 36.0f) % 10;

	return FString::FromInt(PositionIndex);
}


void ACombinationLockAlphabetActor::CheckRightAnswer()
{
	if(CheckPassword(GetCurrentDial()))
	{
		Unlock(TargetDoor);
		CheckAnim = true;
		for(UStaticMeshComponent* InMesh : WheelMeshArray)
		{
			InMesh->SetSimulatePhysics(true);
		}

		SetInstructionWidgetText(FText::FromString(Password), FLinearColor::Blue);
		
		DestroyHitBoxAndWidgetDelayFunction(1.0);
		TargetDoor->DestroyHitBoxAndWidgetDelayFunction(1.0);
	}
}
