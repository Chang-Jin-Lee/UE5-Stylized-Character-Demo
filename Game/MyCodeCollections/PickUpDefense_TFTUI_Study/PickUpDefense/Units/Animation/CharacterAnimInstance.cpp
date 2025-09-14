// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterAnimInstance.h"

void UCharacterAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	CurrentState = ECharacterState::Idle;
}

void UCharacterAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	switch (CurrentState)
	{
	case ECharacterState::Idle:
		PlayIdleAnimation();
		break;

	case ECharacterState::Walking:
		PlayWalkAnimation();
		break;

	// case ECharacterState::Attacking:
	// 	PlayAttackAnimation();
		// break;
	}
}

void UCharacterAnimInstance::PlayIdleAnimation()
{
	if (IdleAnimation)
	{
		PlaySlotAnimationAsDynamicMontage(IdleAnimation, "DefaultSlot", 0.2f, 0.2f);
	}
}

void UCharacterAnimInstance::PlayWalkAnimation()
{
	if (WalkAnimation)
	{
		PlaySlotAnimationAsDynamicMontage(WalkAnimation, "DefaultSlot", 0.2f, 0.2f);
	}
}

// void UCharacterAnimInstance::PlayAttackAnimation()
// {
// 	if (AttackAnimation)
// 	{
// 		Montage_Play(AttackAnimation);
// 	}
// }
