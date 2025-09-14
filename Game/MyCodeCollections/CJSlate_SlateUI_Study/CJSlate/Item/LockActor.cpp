// Fill out your copyright notice in the Description page of Project Settings.


#include "LockActor.h"

#include "Camera/CameraComponent.h"
#include "CJSlate/Managers/AnswerManager.h"
#include "CJSlate/UI/Lock/LockDirectionWidget.h"
#include "Components/WidgetComponent.h"
#include "Components/WidgetInteractionComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"


// Sets default values
ALockActor::ALockActor()
{
	PrimaryActorTick.bCanEverTick = true;
	
	bReplicates = true;
	SetReplicateMovement(true);

	LockRootSceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("LockRootSceneComponent"));
	LockRootSceneComponent->SetupAttachment(SceneComponent);

	LockDirectionWidgetsSceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("LockDirectionWidgetsSceneComponent"));
	LockDirectionWidgetsSceneComponent->SetupAttachment(LockRootSceneComponent);

	LockDirectionWidgetInteraction = CreateDefaultSubobject<UWidgetInteractionComponent>(TEXT("LockDirectionWidgetInteraction"));
	LockDirectionWidgetInteraction->SetupAttachment(LockRootSceneComponent);

	// LockDirectionWidgetInteraction->bShowDebug = true;	// 디버그용
	LockDirectionWidgetInteraction->InteractionSource = EWidgetInteractionSource::Mouse;

	// Directional Lock : 위 오른쪽 아래 왼쪽
	// Combination Lock : 인덱스 순서대로
	LockDirectionWidgetComponents.SetNum(8);
	for (int32 i = 0; i < 8; ++i)
	{
		FString WidgetName = FString::Printf(TEXT("LockDirectionWidgetComponent%d"), i);
		LockDirectionWidgetComponents[i] = CreateDefaultSubobject<UWidgetComponent>(*WidgetName);
		LockDirectionWidgetComponents[i]->SetupAttachment(LockDirectionWidgetsSceneComponent);
		LockDirectionWidgetComponents[i]->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}

	/////////////////// Directional Lock
	DirectionalLockSceneComponent  = CreateDefaultSubobject<USceneComponent>(TEXT("DirectionalLockSceneComponent"));
	DirectionalLockSceneComponent->SetupAttachment(LockRootSceneComponent);

	DirectionalLockshackleMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DirectionalLockshackleMeshComponent"));
	DirectionalLockshackleMeshComponent->SetupAttachment(DirectionalLockSceneComponent);

	DirectionalLockBodyMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DirectionalLockBodyMeshComponent"));
	DirectionalLockBodyMeshComponent->SetupAttachment(DirectionalLockSceneComponent);

	DirectionalLockBallMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DirectionalLockBallMeshComponent"));
	DirectionalLockBallMeshComponent->SetupAttachment(DirectionalLockBodyMeshComponent);
	DirectionalLockBallMeshComponent->SetRelativeLocation(FVector(0,10.0f,0));
	
	/////////////////// Combination Lock
	WheelSceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("WheelSceneComponent"));
	WheelSceneComponent->SetupAttachment(LockRootSceneComponent);

	WheelMeshes.SetNum(7);
	for (int32 i = 0; i < 7; ++i)
	{
		FString ComponentName = FString::Printf(TEXT("CombinationWheelMesh%d"), i);
		WheelMeshes[i] = CreateDefaultSubobject<UStaticMeshComponent>(*ComponentName);
		WheelMeshes[i]->SetupAttachment(WheelSceneComponent);
	}

	/////////////////// Button Lock
	ButtonDataArray.SetNum(m_iButtonNums);
	MoveTimelines.SetNum(m_iButtonNums);
	ButtonMeshComponents.SetNum(m_iButtonNums);
	
	ButtonLockSceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("ButtonLockSceneComponent"));
	if(ButtonLockSceneComponent)
		ButtonLockSceneComponent->SetupAttachment(LockRootSceneComponent);

	ButtonLockShackleMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ButtonLockShackleMeshComponent"));
	if(ButtonLockShackleMeshComponent)
		ButtonLockShackleMeshComponent->SetupAttachment(ButtonLockSceneComponent);

	ButtonLockBodyMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ButtonLockBodyMeshComponent"));
	if(ButtonLockBodyMeshComponent)
		ButtonLockBodyMeshComponent->SetupAttachment(ButtonLockSceneComponent);

	ButtonLockKeypadMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ButtonLockKeypadMeshComponent"));
	if(ButtonLockKeypadMeshComponent)
		ButtonLockKeypadMeshComponent->SetupAttachment(ButtonLockSceneComponent);

	// 카메라
	//CameraMoveTimelineComponent = CreateDefaultSubobject<UTimelineComponent>(TEXT("CameraMoveTimelineComponent"));

	if(ButtonMeshComponents.Num() > 0)
	{
		for(int8 i = 0; i < m_iButtonNums; ++i)
		{
			ButtonMeshComponents[i] = CreateDefaultSubobject<UStaticMeshComponent>(*FString::Printf(TEXT("ButtonLockButtonComponent_%d"), i));
			ButtonMeshComponents[i]->SetupAttachment(ButtonLockBodyMeshComponent);
			ButtonMeshComponents[i]->SetRelativeLocation(FVector(0,4,0));
			MoveTimelines[i] = CreateDefaultSubobject<UTimelineComponent>(*FString::Printf(TEXT("ButtonLockButtonTimeline_%d"), i));
		}
	}
	
	MoveTimeLine = CreateDefaultSubobject<UTimelineComponent>("MoveTimeLine");
	m_bIsTimeLinePlaying = false;
}

void ALockActor::BeginPlay()
{
	Super::BeginPlay();
	
	if(m_uMoveTimelineCurve)
	{
		// Directional
		m_fMoveTimeLineCallback.BindUFunction(this, FName("MoveMeshLerp"));
		MoveTimeLine->AddInterpFloat(m_uMoveTimelineCurve, m_fMoveTimeLineCallback);
		m_fMoveTimeLineFinishedCallback.BindUFunction(this,FName("MoveFinished"));
		MoveTimeLine->SetTimelineFinishedFunc(m_fMoveTimeLineFinishedCallback);

		// Button
		if(MoveTimelines.Num() > 0)
		{
			for(int8 i = 0; i < m_iButtonNums; ++i)
			{
				if(MoveTimelines[i])
				{
					FOnTimelineFloatStatic ButtonLockButtonMoveStartCallback;
					ButtonLockButtonMoveStartCallback.BindUFunction(this, FName("MoveMeshLerp"), i);
					MoveTimelines[i]->AddInterpFloat(m_uMoveTimelineCurve, ButtonLockButtonMoveStartCallback);
			
					FOnTimelineEvent ButtonLockButtonMoveFinishedCallback;
					ButtonLockButtonMoveFinishedCallback.BindUFunction(this, FName("MoveFinished"));
					MoveTimelines[i]->SetTimelineFinishedFunc(ButtonLockButtonMoveFinishedCallback);
				}
			}
		}
	}
	
	const FGameplayTag DirectionalTag = FGameplayTag::RequestGameplayTag(FName("Item.Lock.Directional"));
	const FGameplayTag CombinationAlphabetTag = FGameplayTag::RequestGameplayTag(FName("Item.Lock.Combination.Alphabet"));
	const FGameplayTag CombinationNumberTag = FGameplayTag::RequestGameplayTag(FName("Item.Lock.Combination.Number"));
	const FGameplayTag ButtonTag = FGameplayTag::RequestGameplayTag(FName("Item.Lock.Button"));
	const FGameplayTagContainer& ItemTags = m_fitemInfo.ItemTag;
	if (ItemTags.HasTag(DirectionalTag))
	{
		for (int32 i = 0; i < 4; ++i)
		{
			if (ULockDirectionWidget* widget = Cast<ULockDirectionWidget>(LockDirectionWidgetComponents[i]->GetWidget()))
			{
				widget->LoadData(this, i);
			}
		}
	}
	else if (ItemTags.HasTag(CombinationAlphabetTag))
	{
		
	}
	else if (ItemTags.HasTag(CombinationNumberTag))
	{
		for (int32 i = 0; i < 6; ++i)
		{
			if (ULockDirectionWidget* widget = Cast<ULockDirectionWidget>(LockDirectionWidgetComponents[i]->GetWidget()))
			{
				widget->LoadData(this, i);
			}
		}
	}
	else if (ItemTags.HasTag(ButtonTag))
	{
		for (int32 i = 0; i < 8; ++i)
		{
			if (ULockDirectionWidget* widget = Cast<ULockDirectionWidget>(LockDirectionWidgetComponents[i]->GetWidget()))
			{
				widget->LoadData(this, i);
			}
		}
	}
}

void ALockActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ALockActor::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(ALockActor, m_aLockAnswerManager);
	DOREPLIFETIME(ALockActor, ButtonDataArray);
}

// InputVector는 Directional Lock에서 키보드 입력을 했을 때 왼쪽, 오른쪽, 위, 아래 키 입렵으로 받을 벡터
// ex) (-1,0);
void ALockActor::MoveFromStart(FVector InputVector, int32 SelectedWheelIndex)
{
	const FGameplayTag LockTag = FGameplayTag::RequestGameplayTag(FName("Item.Lock"));
	const FGameplayTag DirectionalTag = FGameplayTag::RequestGameplayTag(FName("Item.Lock.Directional"));
	const FGameplayTag CombinationTag = FGameplayTag::RequestGameplayTag(FName("Item.Lock.Combination"));
	// const FGameplayTag CombinationAlphabetTag = FGameplayTag::RequestGameplayTag(FName("Item.Lock.Combination.Alphabet"));
	// const FGameplayTag CombinationNumberTag = FGameplayTag::RequestGameplayTag(FName("Item.Lock.Combination.Number"));
	const FGameplayTag ButtonTag = FGameplayTag::RequestGameplayTag(FName("Item.Lock.Button"));
	const FGameplayTagContainer& ItemTags = m_fitemInfo.ItemTag;
	
	if (ItemTags.HasTag(LockTag))
	{
		if (ItemTags.HasTag(DirectionalTag))
		{
			if(m_bIsTimeLinePlaying) return;
			
			m_fInitialLocation = DirectionalLockBallMeshComponent->GetRelativeLocation();
			m_fTargetLocation = m_fInitialLocation + InputVector * m_fDirectionMoveLocationScale;
	
			if(MoveTimeLine)
			{
				m_bIsTimeLinePlaying = true;
				MoveTimeLine->PlayFromStart();
			}
			// 키 입력 받아서 왼쪽 오른쪽 위 아래 넣기
			// LockAnswerManager->AddAnswer();
			if (HasAuthority() && m_aLockAnswerManager)
			{
				m_aLockAnswerManager->AddAnswer(SelectedWheelIndex);
				m_aLockAnswerManager->IsCorrectAnswer();
			}
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("LockActor %d"), SelectedWheelIndex));
		}
		else if (ItemTags.HasTag(CombinationTag))
		{
			if(m_bIsTimeLinePlaying || !WheelMeshes[SelectedWheelIndex]) return;

			m_iSelectedButtonIndex = SelectedWheelIndex;
			m_fInitialRotation = WheelMeshes[m_iSelectedButtonIndex]->GetRelativeRotation();
			m_fTargetRotation.Roll = FMath::Fmod(m_fInitialRotation.Roll + m_fRotationDelta.Roll + 360.0f, 360.0f);
			if (MoveTimeLine)
			{
				m_bIsTimeLinePlaying = true;
				MoveTimeLine->PlayFromStart();
			}
			// LockAnswerManager->BitmaskAnswer();
			
			if (HasAuthority() && m_aLockAnswerManager)
			{
				m_aLockAnswerManager->BitmaskValueAnswer(SelectedWheelIndex, FMath::RoundToInt(m_fTargetRotation.Roll) / 36);
				m_aLockAnswerManager->IsCorrectAnswer();
			}
			
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("LockActor %d"), m_iSelectedButtonIndex));
		}
		else if (ItemTags.HasTag(ButtonTag))
		{
			// 한개라도 움직이는 중이라면 실행하지 않음
			for(FButtonLockButtonData otherButtonMove : ButtonDataArray)
				if(otherButtonMove.bIsButtonMove == true)
					return;

			m_iSelectedButtonIndex = SelectedWheelIndex;
			ButtonDataArray[SelectedWheelIndex].InitialLocation = ButtonMeshComponents[SelectedWheelIndex]->GetRelativeLocation();
			ButtonDataArray[SelectedWheelIndex].bIsButtonMove = true;
			
			if (ButtonDataArray[SelectedWheelIndex].IsCliked == false)
			{
				ButtonDataArray[SelectedWheelIndex].IsCliked = true;
				ButtonDataArray[SelectedWheelIndex].TargetLocation = ButtonDataArray[SelectedWheelIndex].InitialLocation + m_fButtonLockButtonMoveOffset;
				MoveTimelines[SelectedWheelIndex]->PlayFromStart();
			}
			else
			{
				ButtonDataArray[SelectedWheelIndex].IsCliked = false;
				ButtonDataArray[SelectedWheelIndex].TargetLocation = ButtonDataArray[SelectedWheelIndex].InitialLocation + (m_fButtonLockButtonMoveOffset * -1);
				MoveTimelines[SelectedWheelIndex]->PlayFromStart();
			}
			// LockAnswerManager->BitmaskAnswer();
			if (HasAuthority() && m_aLockAnswerManager)
			{
				// m_aLockAnswerManager->AddAnswer(SelectedWheelIndex);
				m_aLockAnswerManager->BitmaskAnswer(SelectedWheelIndex, ButtonDataArray[SelectedWheelIndex].IsCliked);
				m_aLockAnswerManager->IsCorrectAnswer();
			}
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("LockActor %d"), m_iSelectedButtonIndex));
		}
	}

	// 사운드 출력. 일단 주석
	// UGameplayStatics::PlaySoundAtLocation(GetWorld(), SoundEffect, GetActorLocation(),1.0f, 2.0f);
}

void ALockActor::MoveMeshLerp(float value, int32 ButtonIndex)
{
	const FGameplayTag LockTag = FGameplayTag::RequestGameplayTag(FName("Item.Lock"));
	const FGameplayTag DirectionalTag = FGameplayTag::RequestGameplayTag(FName("Item.Lock.Directional"));
	const FGameplayTag CombinationTag = FGameplayTag::RequestGameplayTag(FName("Item.Lock.Combination"));
	// const FGameplayTag CombinationAlphabetTag = FGameplayTag::RequestGameplayTag(FName("Item.Lock.Combination.Alphabet"));
	// const FGameplayTag CombinationNumberTag = FGameplayTag::RequestGameplayTag(FName("Item.Lock.Combination.Number"));
	const FGameplayTag ButtonTag = FGameplayTag::RequestGameplayTag(FName("Item.Lock.Button"));

	const FGameplayTagContainer& ItemTags = m_fitemInfo.ItemTag;
	
	if (ItemTags.HasTag(LockTag))
	{
		if (ItemTags.HasTag(DirectionalTag))
		{
			if(DirectionalLockBallMeshComponent)
			{
				FVector NewLocation = FMath::Lerp(m_fInitialLocation, m_fTargetLocation, value);
				DirectionalLockBallMeshComponent->SetRelativeLocation(NewLocation);
			}
		}
		else if (ItemTags.HasTag(CombinationTag))
		{
			if(WheelMeshes[m_iSelectedButtonIndex])
			{
				FRotator NewRotation = FMath::Lerp(m_fInitialRotation, m_fTargetRotation, value);
				WheelMeshes[m_iSelectedButtonIndex]->SetRelativeRotation(NewRotation);
			}
		}
		else if (ItemTags.HasTag(ButtonTag))
		{
			if (ButtonIndex == INDEX_NONE || ButtonIndex >= ButtonDataArray.Num())
				return;
	
			FVector NewLocation = FMath::Lerp(ButtonDataArray[ButtonIndex].InitialLocation, ButtonDataArray[ButtonIndex].TargetLocation, value);
			if(ButtonMeshComponents.Num()>=ButtonIndex)
				ButtonMeshComponents[ButtonIndex]->SetRelativeLocation(NewLocation);
		}
	}
}

void ALockActor::MoveFinished()
{
	m_bIsTimeLinePlaying = false;

	const FGameplayTag LockTag = FGameplayTag::RequestGameplayTag(FName("Item.Lock"));
	const FGameplayTag DirectionalTag = FGameplayTag::RequestGameplayTag(FName("Item.Lock.Directional"));
	const FGameplayTag CombinationTag = FGameplayTag::RequestGameplayTag(FName("Item.Lock.Combination"));
	// const FGameplayTag CombinationAlphabetTag = FGameplayTag::RequestGameplayTag(FName("Item.Lock.Combination.Alphabet"));
	// const FGameplayTag CombinationNumberTag = FGameplayTag::RequestGameplayTag(FName("Item.Lock.Combination.Number"));
	const FGameplayTag ButtonTag = FGameplayTag::RequestGameplayTag(FName("Item.Lock.Button"));

	const FGameplayTagContainer& ItemTags = m_fitemInfo.ItemTag;
					
	if (ItemTags.HasTag(LockTag))
	{
		if (ItemTags.HasTag(DirectionalTag))
		{
			if (m_bReturning == false)
			{  
				FVector t = m_fInitialLocation;
				m_fInitialLocation = m_fTargetLocation;
				m_fTargetLocation = t;
				m_bReturning = true;
				if(MoveTimeLine)
				{
					m_bIsTimeLinePlaying = true;
					MoveTimeLine->PlayFromStart();
				}
			}
			else
			{
				m_bReturning = false;
				m_bIsTimeLinePlaying = false;
			}
		}
		else if (ItemTags.HasTag(CombinationTag))
		{
			m_bIsTimeLinePlaying = false;
		}
		else if (ItemTags.HasTag(ButtonTag))
		{
			if (m_iSelectedButtonIndex == INDEX_NONE || m_iSelectedButtonIndex >= ButtonDataArray.Num())
				return;
			ButtonDataArray[m_iSelectedButtonIndex].bIsButtonMove = false;
		}
	}
}

