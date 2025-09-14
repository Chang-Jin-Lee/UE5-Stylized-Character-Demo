// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "CharacterAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class PICKUPDEFENSE_API UCharacterAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	// 애니메이션 상태 열거형 정의
	enum class ECharacterState
	{
		Idle,
		Walking,
		Attacking
	};

	// 현재 캐릭터 상태
	ECharacterState CurrentState;

	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	void PlayIdleAnimation();
	void PlayWalkAnimation();
	// void PlayAttackAnimation();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animations")
	UAnimSequence* IdleAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animations")
	UAnimSequence* WalkAnimation;

	// UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animations", meta = (AllowPrivateAccess = "true"))
	// UAnimSequence* AttackAnimation;
};
