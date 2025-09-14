// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LockActor.h"
#include "Components/TimelineComponent.h"
#include "Lena/Data/DataStructure.h"
#include "ButtonLockActor.generated.h"

UCLASS()

class LENA_API AButtonLockActor : public ALockActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AButtonLockActor();

	virtual void Unlock(AActor* ActorToUnlock) override;
	
	UFUNCTION(BlueprintCallable)
	void MoveButton(UStaticMeshComponent* TargetMeshComponent);
	
	UFUNCTION(BlueprintCallable)
	FString GetPressedPassword();
	
	UFUNCTION(BlueprintCallable)
	void OpenLock();
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	// Button Lock
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
	TArray<TObjectPtr<UTimelineComponent>> ButtonTimelines;

	UPROPERTY(VisibleAnywhere, Category="ButtonLock")
	TArray<FButtonLockButtonData> ButtonDataArray;

	int32 ButtonNums = 8;

	UPROPERTY(EditAnywhere, Category="TimeLine")
	TObjectPtr<UCurveFloat> ButtonLockCurve;
	
	UFUNCTION()
	void HandleButtonLockProgress(float value, int32 ButtonIndex);
	
	UFUNCTION()
	void HandleButtonLockFinished();

	UPROPERTY()
	FVector InitialLocation;

	UPROPERTY()
	FVector TargetLocation;

	int32 SelectedButtonIndex = 0;

	void ButtonMovePlayFromStart(int32 index, float value);
	void DestroyButtonLock();
	
	// Camera
	UPROPERTY(EditAnywhere, Category="Camera")
	TObjectPtr<UTimelineComponent> CameraMoveTimelineComponent;

	UPROPERTY(EditAnywhere, Category="Camera")
	TObjectPtr<UCurveFloat> CameraMoveCurve;

	UPROPERTY(VisibleAnywhere, Category="Camera")
	FOnTimelineFloat CameraMoveCallback;

	UPROPERTY(VisibleAnywhere, Category="Camera")
	FOnTimelineEvent CameraMoveFinishedCallback;
};
