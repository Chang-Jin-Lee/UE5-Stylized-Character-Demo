// Fill out your copyright notice in the Description page of Project Settings.


#include "Lena_IntroGamePlayerController.h"

void ALena_IntroGamePlayerController::BeginPlay()
{
	Super::BeginPlay();

	bShowMouseCursor = true;
	FInputModeGameAndUI inputmode;
	SetInputMode(inputmode);
}
