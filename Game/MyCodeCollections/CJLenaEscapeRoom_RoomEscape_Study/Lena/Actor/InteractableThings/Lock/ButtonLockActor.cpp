// Fill out your copyright notice in the Description page of Project Settings.


#include "Lena/Actor/InteractableThings/Lock/ButtonLockActor.h"

#include "Lena/Characters/Base_Character.h"
#include "Components/BoxComponent.h"
#include "Lena/Controller/ShooterPlayerController.h"
#include "Lena/Actor/InteractableThings/Door/DoorActor.h"
#include "Kismet/GameplayStatics.h"
#include "Lena/GameInstanceSubSystem/LenaRoom01Manager.h"

// Sets default values
AButtonLockActor::AButtonLockActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	ButtonDataArray.SetNum(ButtonNums);
	ButtonTimelines.SetNum(ButtonNums);
	ButtonMeshComponents.SetNum(ButtonNums);
	
	ButtonLockSceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("ButtonLockSceneComponent"));
	if(ButtonLockSceneComponent)
		ButtonLockSceneComponent->SetupAttachment(RootComponent);

	ButtonLockShackleMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ButtonLockShackleMeshComponent"));
	if(ButtonLockShackleMeshComponent)
		ButtonLockShackleMeshComponent->SetupAttachment(ButtonLockSceneComponent);

	ButtonLockBodyMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ButtonLockBodyMeshComponent"));
	if(ButtonLockBodyMeshComponent)
		ButtonLockBodyMeshComponent->SetupAttachment(ButtonLockSceneComponent);

	ButtonLockKeypadMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ButtonLockKeypadMeshComponent"));
	if(ButtonLockKeypadMeshComponent)
		ButtonLockKeypadMeshComponent->SetupAttachment(ButtonLockSceneComponent);

	CameraMoveTimelineComponent = CreateDefaultSubobject<UTimelineComponent>(TEXT("CameraMoveTimelineComponent"));

	if(ButtonMeshComponents.Num() > 0)
	{
		for(int8 i = 0; i < ButtonNums; ++i)
		{
			// ButtonData = NewObject<UButtonLockButtonData>(this);
			ButtonMeshComponents[i] = CreateDefaultSubobject<UStaticMeshComponent>(*FString::Printf(TEXT("ButtonLockButtonComponent_%d"), i));
			ButtonMeshComponents[i]->SetupAttachment(ButtonLockBodyMeshComponent);
			ButtonMeshComponents[i]->SetRelativeLocation(FVector(0,4,0));
			ButtonTimelines[i] = CreateDefaultSubobject<UTimelineComponent>(*FString::Printf(TEXT("ButtonLockButtonTimeline_%d"), i));
		}
	}

	if(HitBox)
	{
		HitBox->SetRelativeLocation(FVector(0,0,20));
		HitBox->SetRelativeScale3D(FVector::One() * 2.0f);
	}
}

void AButtonLockActor::Unlock(AActor* ActorToUnlock)
{
	Super::Unlock(ActorToUnlock);
	ADoorActor* Door = Cast<ADoorActor>(ActorToUnlock);
	if(Door)
	{
		Door->RequiredCondition = "Default";
		Door->Open();
	}
}

// Called when the game starts or when spawned
void AButtonLockActor::BeginPlay()
{
	Super::BeginPlay();

	if(ButtonTimelines.Num() > 0)
	{
		for(int8 i = 0; i < ButtonNums; ++i)
		{
			if(ButtonTimelines[i])
			{
				FOnTimelineFloatStatic ButtonLockButtonMoveStartCallback;
				ButtonLockButtonMoveStartCallback.BindUFunction(this, FName("HandleButtonLockProgress"), i);
				ButtonTimelines[i]->AddInterpFloat(ButtonLockCurve, ButtonLockButtonMoveStartCallback);
			
				FOnTimelineEvent ButtonLockButtonMoveFinishedCallback;
				// ButtonLockButtonMoveFinishedCallback.CreateUFunction(this, FName("HandleButtonLockFinished"), i);
				ButtonLockButtonMoveFinishedCallback.BindUFunction(this, FName("HandleButtonLockFinished"));
				ButtonTimelines[i]->SetTimelineFinishedFunc(ButtonLockButtonMoveFinishedCallback);
			}
		}
	}
}

// Called every frame
void AButtonLockActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AButtonLockActor::HandleButtonLockProgress(float value, int32 ButtonIndex)
{
	if (ButtonIndex == INDEX_NONE || ButtonIndex >= ButtonDataArray.Num())
		return;
	
	FVector NewLocation = FMath::Lerp(ButtonDataArray[ButtonIndex].InitialLocation, ButtonDataArray[ButtonIndex].TargetLocation, value);
	if(ButtonMeshComponents.Num()>=ButtonIndex)
		ButtonMeshComponents[ButtonIndex]->SetRelativeLocation(NewLocation);
}

void AButtonLockActor::MoveButton(UStaticMeshComponent* TargetMeshComponent)
{
	int32 FindIndex = ButtonMeshComponents.IndexOfByKey(TargetMeshComponent);
	
	if (FindIndex == INDEX_NONE)
		return;

	for(FButtonLockButtonData otherButtonMove : ButtonDataArray)
		if(otherButtonMove.bIsButtonMove == true)
			return;

	SelectedButtonIndex = FindIndex;
	ButtonDataArray[FindIndex].InitialLocation = TargetMeshComponent->GetRelativeLocation();
	ButtonDataArray[FindIndex].bIsButtonMove = true;
	ButtonDataArray[FindIndex].IsCliked == true ?  ButtonMovePlayFromStart(FindIndex, 4) : ButtonMovePlayFromStart(FindIndex, -4);
	ButtonDataArray[FindIndex].IsCliked = !ButtonDataArray[FindIndex].IsCliked;

	UGameplayStatics::PlaySoundAtLocation(GetWorld(), SoundEffect, GetActorLocation());

	if(CheckPassword(GetPressedPassword()))
		OpenLock();
}

void AButtonLockActor::ButtonMovePlayFromStart(int32 index, float value)
{
	ButtonDataArray[index].TargetLocation = ButtonDataArray[index].InitialLocation + FVector(0,value,0);
	ButtonTimelines[index]->PlayFromStart();
}

void AButtonLockActor::HandleButtonLockFinished()
{
	if (SelectedButtonIndex == INDEX_NONE || SelectedButtonIndex >= ButtonDataArray.Num())
		return;
	ButtonDataArray[SelectedButtonIndex].bIsButtonMove = false;
}

FString AButtonLockActor::GetPressedPassword()
{
	FString string = "";
	for(int32 i = 0; i < ButtonNums; ++i)
	{
		if(ButtonDataArray[i].IsCliked)
		{
			string+=FString::FromInt(i+1);
		}
	}
	return string;
}

void AButtonLockActor::OpenLock()
{
	Unlock(TargetDoor);
	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(
		TimerHandle,
		this,
		&AButtonLockActor::DestroyButtonLock,
		1.0f,
		false
		);
	
	ButtonLockShackleMeshComponent->SetSimulatePhysics(true);
	ButtonLockBodyMeshComponent->SetSimulatePhysics(true);
	ButtonLockKeypadMeshComponent->SetSimulatePhysics(true);
	for(int32 i = 0; i < ButtonNums; ++i)
	{
		ButtonMeshComponents[i]->SetSimulatePhysics(true);
	}
	WidgetComponent->SetVisibility(false);
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), RightAnswerSound, GetActorLocation());
}

void AButtonLockActor::DestroyButtonLock()
{
	DestroyHitBoxAndWidgetDelayFunction(1.0);
	if(TargetDoor)
		TargetDoor->DestroyHitBoxAndWidgetDelayFunction(1.0);
}
