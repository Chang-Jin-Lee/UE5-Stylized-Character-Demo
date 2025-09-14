// Fill out your copyright notice in the Description page of Project Settings.


#include "AdvancedPlayerCameraManager.h"

#include "Lena/GameInstanceSubSystem/LenaRoom01Manager.h"

AAdvancedPlayerCameraManager::AAdvancedPlayerCameraManager()
{
	
}

void AAdvancedPlayerCameraManager::BeginPlay()
{
	Super::BeginPlay();

	if(g_LenaRoom01ManagerIsValid == true)
	{
		g_LenaRoom01Manager->SetCameraManager(this);
	}
}

void AAdvancedPlayerCameraManager::ZoomInCamera()
{
	if(bIsZoomedIn) return;
	LockCamera();
	bIsZoomedIn = true;
	if(PlayerCharacter)
	{
		PlayerCharacter->SetActorHiddenInGame(true);
	}
	
	if (CameraTargetLocationActor)
	{
		MoveCamera(CameraTargetLocationActor);
	}
}

void AAdvancedPlayerCameraManager::ZoomOutCamera()
{
	if(bIsZoomedIn) return;
	bIsZoomedIn = true;
	if(PlayerCharacter)
	{
		PlayerCharacter->SetActorHiddenInGame(false);
	}
	MoveCamera(PlayerController->GetPawn());
	UnLockCameara();
}

void AAdvancedPlayerCameraManager::LockCamera()
{
	if(PlayerController)
	{
		PlayerController->bShowMouseCursor = true;
		PlayerController->SetIgnoreLookInput(true);
		PlayerController->SetIgnoreMoveInput(true);
		PlayerController->SetInputMode(FInputModeGameAndUI());

		AShooterPlayerController* shooter_player_controller = Cast<AShooterPlayerController>(PlayerController);
		if(shooter_player_controller)
		{
			shooter_player_controller->EnableMouseClick();
		}
	}
}

void AAdvancedPlayerCameraManager::UnLockCameara()
{
	if (PlayerController)
	{
		PlayerController->bShowMouseCursor = false;
		PlayerController->SetIgnoreLookInput(false);
		PlayerController->SetIgnoreMoveInput(false);
		PlayerController->SetInputMode(FInputModeGameOnly());
		
		AShooterPlayerController* shooter_player_controller = Cast<AShooterPlayerController>(PlayerController);
		if(shooter_player_controller)
		{
			shooter_player_controller->DisableMouseClick();
		}
	}
}

// Create Target Actor for SetViewTargetWithBlend in Camera Zoomin
void AAdvancedPlayerCameraManager::CreateCameraMoveTargetLocationActor(TObjectPtr<AActor> _actor)
{
	if(PlayerCharacter == nullptr || PlayerController == nullptr)
	{
		if(g_LenaRoom01ManagerIsValid == true)
		{
			PlayerController = g_LenaRoom01Manager->GetPlayerController();
			PlayerCharacter = g_LenaRoom01Manager->GetCharacter();
		}
	}
	if(PlayerController)
	{
		ZoomedCameraLocation = _actor->GetActorLocation() + _actor->GetActorRightVector() * 100.0f * _actor->GetActorScale() + FVector(0, 0.f, 10.0f) *  _actor->GetActorScale();
		ZoomedCameraRotation = _actor->GetActorRotation();
		ZoomedCameraRotation.Yaw += -90.0f;
		ZoomedCameraRotation.Pitch += -10.0f;
	}
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	AActor* CameraTargetActor = GetWorld()->SpawnActor<AActor>(AActor::StaticClass(), SpawnParams);
	if (CameraTargetActor)
	{
		USceneComponent* SceneComponent = NewObject<USceneComponent>(CameraTargetActor);
		if (SceneComponent)
		{
			CameraTargetActor->SetRootComponent(SceneComponent);
			SceneComponent->RegisterComponent();
			SceneComponent->SetWorldLocation(ZoomedCameraLocation);
			SceneComponent->SetWorldRotation(ZoomedCameraRotation);
		}
	}
	CameraTargetLocationActor = CameraTargetActor;
}

void AAdvancedPlayerCameraManager::MoveCamera(TObjectPtr<AActor> TargetActor)
{
	if (PlayerController)
	{
		FViewTargetTransitionParams TransitionParams;
		TransitionParams.BlendTime = 1.0f; // Adjust transition time as needed
		PlayerController->SetViewTargetWithBlend(TargetActor, TransitionParams.BlendTime, VTBlend_Cubic);

		FTimerHandle TimerHandle;
		GetWorld()->GetTimerManager().SetTimer(
			TimerHandle,
			this,
			&AAdvancedPlayerCameraManager::CameraMoveFinished,
			TransitionParams.BlendTime,
			false
			);
	}
}

void AAdvancedPlayerCameraManager::CameraMoveFinished()
{
	bIsZoomedIn = false;
}