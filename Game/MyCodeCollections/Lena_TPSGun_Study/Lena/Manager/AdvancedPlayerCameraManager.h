// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/PlayerCameraManager.h"
#include "Lena/Characters/Base_Character.h"
#include "Lena/Controller/ShooterPlayerController.h"
#include "AdvancedPlayerCameraManager.generated.h"

class USpotLightComponent;
/**
 * 
 */
UCLASS()
class LENA_API AAdvancedPlayerCameraManager : public APlayerCameraManager
{
	GENERATED_BODY()

public:
	AAdvancedPlayerCameraManager();

	virtual void BeginPlay() override;
	void ZoomInCamera();
	void ZoomOutCamera();
	void LockCamera();
	void UnLockCameara();
	void CreateCameraMoveTargetLocationActor(TObjectPtr<AActor> _actor);
	void MoveCamera(TObjectPtr<AActor> TargetActor);
	void CameraMoveFinished();
	UFUNCTION(BlueprintImplementableEvent)
	void OnPossesssCameraZoomed();

	UPROPERTY(BlueprintReadOnly)
	int PublicValue = 10;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<USpotLightComponent> SpotLight;

	FORCEINLINE bool GetbIsZoomedIn() { return bIsZoomedIn; }

private:
	UPROPERTY()
	TObjectPtr<APawn> AdvancedControlledPawn;
	bool bIsZoomedIn = false;
	UPROPERTY()
	TObjectPtr<ABase_Character> PlayerCharacter;
	UPROPERTY()
	TObjectPtr<AShooterPlayerController> PlayerController;
	UPROPERTY()
	TObjectPtr<AActor> CameraTargetLocationActor;
	UPROPERTY()
	TObjectPtr<AActor> CameraInitialLocationActor;
	FVector ZoomedCameraLocation;
	FRotator ZoomedCameraRotation;
};
