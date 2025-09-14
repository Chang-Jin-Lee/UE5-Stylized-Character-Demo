// Fill out your copyright notice in the Description page of Project Settings.


#include "TEST_PlayerController.h"
#include "EnhancedInputComponent.h"
#include "InputAction.h"
#include "InputMappingContext.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/Character.h"

ATEST_PlayerController::ATEST_PlayerController()
{
	
}

void ATEST_PlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		Subsystem->ClearAllMappings();
		Subsystem->AddMappingContext(IMC_MappingContext, 0);
	}

	if (UEnhancedInputComponent* EIP = Cast<UEnhancedInputComponent>(InputComponent))
	{
		EIP->BindAction(IA_MoveAction, ETriggerEvent::Triggered, this, &ATEST_PlayerController::MoveAction);
	}
}

void ATEST_PlayerController::MoveAction(const FInputActionInstance& Instance)
{
	FVector2d DirectionVector = Instance.GetValue().Get<FVector2d>();
	
	const FRotator Rotation = GetControlRotation();
	const FRotator YawRotation(0,Rotation.Yaw,0);
	
	FVector FowardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
	
	GetCharacter()->AddMovementInput(FowardDirection, DirectionVector.X);
	GetCharacter()->AddMovementInput(RightDirection, DirectionVector.Y);
}
