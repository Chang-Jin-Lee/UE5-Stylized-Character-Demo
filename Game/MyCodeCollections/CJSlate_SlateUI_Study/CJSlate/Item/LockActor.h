// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemActor.h"
#include "Components/TimelineComponent.h"
#include "LockActor.generated.h"

class UWidgetInteractionComponent;
class AAnswerManager;
class UTimelineComponent;

UENUM(BlueprintType)
enum class EDirectionType : uint8
{
	None UMETA(DisplayName = "None"),
	Right UMETA(DisplayName = "→"),
	Left UMETA(DisplayName = "←"),
	Up UMETA(DisplayName = "↑"),
	Down UMETA(DisplayName = "↓")
};

USTRUCT(BlueprintType)
struct FButtonLockButtonData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsButtonMove;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool IsCliked = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector InitialLocation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector TargetLocation;

	FButtonLockButtonData()
		: bIsButtonMove(false)
		, IsCliked(false)
		, InitialLocation(FVector::ZeroVector)
		, TargetLocation(FVector::ZeroVector)
	{
	}
};

// 자물쇠의 기본이 되는 액터. 어떤 자물쇠인지는 GameplayTag로 규정.
UCLASS()
class CJSLATE_API ALockActor : public AItemActor
{
	GENERATED_BODY()

public:
	ALockActor();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

	// 현현재 자물쇠에 대한 정답을 관리할 매니저
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Replicated)
	TObjectPtr<AAnswerManager> m_aLockAnswerManager;

	// 모든 SceneComponent의 부모. 즉 RootSceneComponent.
	UPROPERTY(VisibleAnywhere, Category="Lock_Root")
	TObjectPtr<USceneComponent> LockRootSceneComponent;

	// 자물쇠의 방향을 클릭하는 UI
	UPROPERTY(VisibleAnywhere, Category="LockWidget")
	TArray<TObjectPtr<UWidgetComponent>> LockDirectionWidgetComponents;

	// 방향 위젯을 모아서 한 곳에 둘 씬 컴포넌트
	UPROPERTY(VisibleAnywhere, Category="Widget_Root")
	TObjectPtr<USceneComponent> LockDirectionWidgetsSceneComponent;

	// 위젯을 클릭하기 위한 인터렉션 컴포넌트
	UPROPERTY(VisibleAnywhere, Category="Widget_Root")
	UWidgetInteractionComponent* LockDirectionWidgetInteraction;

	// * Directional Lock 에서 쓰이는 컴포넌트들 */
	void MoveFromStart(FVector InputVector, int32 SelectedWheelIndex = -1); // 움직임을 나타내는 구분자. ex) DirectionalLock은 방향, Combination은 Rotaiton 등.
	// UFUNCTION(Server, Reliable, WithValidation)
	// void MoveFromStart(FVector InputVector, int32 SelectedWheelIndex = -1); // 움직임을 나타내는 구분자. ex) DirectionalLock은 방향, Combination은 Rotaiton 등.
	
	UPROPERTY(VisibleAnywhere, Category="DirectionalLock")
	TObjectPtr<USceneComponent> DirectionalLockSceneComponent;

	UPROPERTY(VisibleAnywhere, Category="DirectionalLock")
	TObjectPtr<UStaticMeshComponent> DirectionalLockBodyMeshComponent;

	UPROPERTY(VisibleAnywhere, Category="DirectionalLock")
	TObjectPtr<UStaticMeshComponent> DirectionalLockshackleMeshComponent;

	UPROPERTY(VisibleAnywhere, Category="DirectionalLock")
	TObjectPtr<UStaticMeshComponent> DirectionalLockBallMeshComponent;
	
	UPROPERTY()
	TObjectPtr<UTimelineComponent> MoveTimeLine;

	UPROPERTY()
	FOnTimelineFloat m_fMoveTimeLineCallback;

	UPROPERTY()
	FOnTimelineEvent m_fMoveTimeLineFinishedCallback;

	UFUNCTION()
	void MoveMeshLerp(float value, int32 ButtonIndex = -1);

	UFUNCTION()
	void MoveFinished();

	UPROPERTY(EditAnywhere, Category="MoveTimeline")
	TObjectPtr<UCurveFloat> m_uMoveTimelineCurve;

	// Directional Lock에서 원 점으로 돌아오고 있는 중인지를 판단하는 변수
	bool m_bReturning = false;

	// TimeLine이 실행되고 있는지 판단.
	bool m_bIsTimeLinePlaying = false;

	UPROPERTY()
	FVector m_fInitialLocation;
	
	UPROPERTY()
	FVector m_fTargetLocation;

	UPROPERTY(EditAnywhere, Category="DirectionalLock")
	FVector m_fDirectionMoveLocationScale;

	// * Combination Lock 에서 쓰이는 컴포넌트들 */
		
	UPROPERTY(VisibleAnywhere, Category="CombinationLock")
	TObjectPtr<USceneComponent> WheelSceneComponent;

	UPROPERTY(VisibleAnywhere, Category="CombinationLock")
	TArray<TObjectPtr<UStaticMeshComponent>> WheelMeshes;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta=(AllowPrivateAccess="true"))
	TObjectPtr<USkeletalMeshComponent> ShackleMesh;
	
	UPROPERTY()
	FRotator m_fInitialRotation;

	UPROPERTY(EditAnywhere)
	FRotator m_fTargetRotation = FRotator(0,0,0);

	UPROPERTY(EditAnywhere)
	FRotator m_fRotationDelta = FRotator(0,0,36);

	// ** Button Lock에서 쓰이는 컴포넌트들 */

	UPROPERTY(VisibleAnywhere, Category="ButtonLock")
	TObjectPtr<USceneComponent> ButtonLockSceneComponent;

	UPROPERTY(VisibleAnywhere, Category="ButtonLock")
	TObjectPtr<UStaticMeshComponent> ButtonLockBodyMeshComponent;

	UPROPERTY(VisibleAnywhere, Category="ButtonLock")
	TObjectPtr<UStaticMeshComponent> ButtonLockKeypadMeshComponent;

	UPROPERTY(VisibleAnywhere, Category="ButtonLock")
	TObjectPtr<UStaticMeshComponent> ButtonLockShackleMeshComponent;

	UPROPERTY(VisibleAnywhere, Category="ButtonLock")
	TArray<TObjectPtr<UStaticMeshComponent>> ButtonMeshComponents;

	UPROPERTY(VisibleAnywhere, Category="ButtonLock")
	TArray<TObjectPtr<UTimelineComponent>> MoveTimelines;

	UPROPERTY(VisibleAnywhere, Category="ButtonLock", Replicated)
	TArray<FButtonLockButtonData> ButtonDataArray;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool m_bIsButtonMove;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector m_fButtonLockButtonMoveOffset = FVector(0,-4,0);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool m_bCliked;
	
	int32 m_iButtonNums = 8;
	int32 m_iSelectedButtonIndex = 0;

	// Camera
	// 이 부분의 로직은 Controller에서 해야함.
	// UPROPERTY(EditAnywhere, Category="Camera")
	// TObjectPtr<UTimelineComponent> CameraMoveTimelineComponent;
	//
	// UPROPERTY(EditAnywhere, Category="Camera")
	// TObjectPtr<UCurveFloat> CameraMoveCurve;
	//
	// UPROPERTY(VisibleAnywhere, Category="Camera")
	// FOnTimelineFloat CameraMoveCallback;
	//
	// UPROPERTY(VisibleAnywhere, Category="Camera")
	// FOnTimelineEvent CameraMoveFinishedCallback;
};
