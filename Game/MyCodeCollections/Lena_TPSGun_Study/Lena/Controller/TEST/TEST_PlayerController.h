// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "TEST_PlayerController.generated.h"

struct FInputActionInstance;
class UInputAction;
class UInputMappingContext;

UCLASS()
class LENA_API ATEST_PlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	ATEST_PlayerController();

	virtual void SetupInputComponent() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Input)
	UInputAction* IA_MoveAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Input)
	TObjectPtr<UInputMappingContext> IMC_MappingContext;

	void MoveAction(const FInputActionInstance& Instance);
};
