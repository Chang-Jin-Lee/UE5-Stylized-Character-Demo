// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "PlayerAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class CHANGJINCODESTORY_API UPlayerAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

public:
	UPROPERTY(BlueprintReadOnly)
	class APlayerCharacter* PlayerCharacter;
	UPROPERTY(BlueprintReadOnly)
	class UCharacterMovementComponent* MovementComponent;

	UPROPERTY(BlueprintReadOnly)
	FVector Velocity;

	UPROPERTY(BlueprintReadOnly)
	float GroundSpeed;

	UPROPERTY(BlueprintReadOnly)
	bool ShouldMove = false;

	UPROPERTY(BlueprintReadOnly)
	bool IsFalling = false;
};
