// Copyright Epic Games, Inc. All Rights Reserved.

#include "TP_TopDownCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "Camera/CameraComponent.h"
#include "Components/DecalComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/SpringArmComponent.h"
#include "Materials/Material.h"
#include "Engine/World.h"

ATP_TopDownCharacter::ATP_TopDownCharacter()
{
	// Set size for player capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Don't rotate character to camera direction
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Rotate character to moving direction
	GetCharacterMovement()->RotationRate = FRotator(0.f, 640.f, 0.f);
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;

	// Create a camera boom...
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->SetUsingAbsoluteRotation(true); // Don't want arm to rotate when character does
	CameraBoom->TargetArmLength = 800.f;
	CameraBoom->SetRelativeRotation(FRotator(-60.f, 0.f, 0.f));
	CameraBoom->bDoCollisionTest = false; // Don't want to pull camera in when it collides with level

	// Create a camera...
	TopDownCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("TopDownCamera"));
	TopDownCameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	TopDownCameraComponent->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Activate ticking in order to update the cursor every frame.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;
}

void ATP_TopDownCharacter::CheckEnemyPosition()
{
	if (!enemy) return;
	
	FVector playerForward = GetActorForwardVector();
	DrawDebugLine(GetWorld(), GetActorLocation(), playerForward*300, FColor::Blue);

	FVector toEnemy = (enemy->GetActorLocation() - GetActorLocation()).GetSafeNormal();
	DrawDebugLine(GetWorld(), GetActorLocation(), toEnemy*300, FColor::White);


	float forwardDot = playerForward.Dot((toEnemy));
	if (forwardDot < 0)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Green, TEXT("Enemy가 Player 뒤에 있습니다."));
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Green, TEXT("Enemy가 Player 앞에 있습니다."));
	}

	FVector cross = playerForward.Cross(toEnemy).GetSafeNormal();
	DrawDebugLine(GetWorld(), GetActorLocation(), cross*300, FColor::Yellow);
	UE_LOG(LogTemp, Warning, TEXT("Clean Map Name: %f"), cross.Y);

	
	if (FMath::Abs((cross.Y)) > 0.001f)
	{
		if (cross.Y > 0)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Green, TEXT("Enemy는 Player의 왼쪽에 있습니다."));
		}
		else
		{
			GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Green, TEXT("Enemy는 Player의 오른쪽에 있습니다."));

		}
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Green, TEXT("Enemy는 정면 또는 후면에 정렬되어 있습니다."));
	}
}

void ATP_TopDownCharacter::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);

	CheckEnemyPosition();
}
