// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/PlayerAnimInstance.h"

#include "Characters/PlayerCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PawnMovementComponent.h"
#include "Kismet/GameplayStatics.h"


void UPlayerAnimInstance::NativeInitializeAnimation()
{
	PlayerCharacter = Cast<APlayerCharacter>(GetOwningActor());
	if(PlayerCharacter)
	{
		MovementComponent = Cast<UCharacterMovementComponent>(PlayerCharacter->GetMovementComponent());
	}
}

void UPlayerAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	if(PlayerCharacter)
	{
		Velocity = MovementComponent->Velocity;
		GroundSpeed = Velocity.Length();
		ShouldMove = GroundSpeed > 3 && MovementComponent->GetCurrentAcceleration() != FVector(0,0,0);
		IsFalling = MovementComponent->IsFalling();
	}
}

