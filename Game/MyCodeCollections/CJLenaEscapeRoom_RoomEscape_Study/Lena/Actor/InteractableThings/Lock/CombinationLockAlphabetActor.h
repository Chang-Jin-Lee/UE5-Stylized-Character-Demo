// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LockActor.h"
#include "Components/TimelineComponent.h"
#include "Lena/Actor/InteractableThings/Door/DoorActor.h"
#include "CombinationLockAlphabetActor.generated.h"


UCLASS()

class LENA_API ACombinationLockAlphabetActor : public ALockActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ACombinationLockAlphabetActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void Unlock(AActor* ActorToUnlock) override;

	UFUNCTION(BlueprintCallable)
	void ScrollCombinationLock(FRotator InTargetRotation);

	UFUNCTION()
	void OnTimeLineFinished();

	UFUNCTION(BlueprintCallable)
	void MoveNextWheelMesh();

	UFUNCTION(BlueprintCallable)
	void MovePrevWheelMesh();
	
	UFUNCTION(BlueprintCallable)
	FString GetCurrentDial();
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool CheckAnim = false;
	
private:

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USceneComponent> WheelSceneComponent;
		
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> WheelMesh;
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> WheelMesh1;
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> WheelMesh2;
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> WheelMesh3;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> WheelMesh4;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> WheelMesh5;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> WheelMesh6;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta=(AllowPrivateAccess="true"))
	TObjectPtr<USkeletalMeshComponent> ShackleMesh;

	// WheelMesh Scale3D
	float WheelMeshDefaultScaleSize = 5.0f;
	float WheelMeshSelectedSize = 0;

	UFUNCTION()
	void HandleCombinationLockProgress(float Value);

	UPROPERTY()
	TObjectPtr<UTimelineComponent> CombinationLockTimeline;

	UPROPERTY()
	FOnTimelineFloat CombinationLockTimelineCallback;
	
	UPROPERTY()
	FOnTimelineEvent TimelineFinishedCallback;

	UPROPERTY()
	FRotator InitialRotation;

	UPROPERTY()
	FRotator TargetRotation;

	UPROPERTY(EditAnywhere, Category = "Timeline")
	TObjectPtr<UCurveFloat> CombinationLockCurve;

	bool bIsTimeLinePlaying = false;

	int SelectedWheelIndex = 0;

	UPROPERTY()
	TArray<TObjectPtr<UStaticMeshComponent>> WheelMeshArray;

	// EWheelMeshEnum SelectedWheelMeshEnum; // Selected Mesh Enum

	FString GetCurrentDial(UStaticMeshComponent* WheelMeshDial);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Door", meta=(AllowPrivateAccess="true"))
	TSubclassOf<class ASlidingDoorActor> DoorActorClass;

	UPROPERTY()
	TObjectPtr<ASlidingDoorActor> DoorActor;

	void CheckRightAnswer();

};
