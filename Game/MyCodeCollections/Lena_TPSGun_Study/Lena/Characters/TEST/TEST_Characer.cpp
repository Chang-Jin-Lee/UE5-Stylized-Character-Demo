// Fill out your copyright notice in the Description page of Project Settings.


#include "TEST_Characer.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"


// Sets default values
ATEST_Characer::ATEST_Characer()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
	if (SpringArmComponent && CameraComponent)
	{
		CameraComponent->SetupAttachment(SpringArmComponent);
		SpringArmComponent->SetupAttachment(GetRootComponent());
	}
}

// Called when the game starts or when spawned
void ATEST_Characer::BeginPlay()
{
	Super::BeginPlay();

	GetMesh()->SetRelativeLocation(FVector(0,0,-78.0f));
	GetMesh()->SetRelativeRotation(FRotator(0,-90,0));
}

// Called every frame
void ATEST_Characer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void ATEST_Characer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

