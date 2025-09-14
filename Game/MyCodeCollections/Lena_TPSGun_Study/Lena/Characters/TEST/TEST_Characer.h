// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "TEST_Characer.generated.h"

class UCameraComponent;
class USpringArmComponent;

UCLASS()
class LENA_API ATEST_Characer : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ATEST_Characer();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Camera")
	USpringArmComponent* SpringArmComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Camera")
	UCameraComponent* CameraComponent;
	
};
