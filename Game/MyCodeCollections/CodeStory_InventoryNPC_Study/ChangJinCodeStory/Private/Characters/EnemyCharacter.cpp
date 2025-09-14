// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/EnemyCharacter.h"

#include "AI/EnemyAIController.h"


// Sets default values
AEnemyCharacter::AEnemyCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<UAnimBlueprint> FindAnimInstance(TEXT("/Game/Characters/ABP_PlayerAnimation.ABP_PlayerAnimation"));
	if (FindAnimInstance.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(FindAnimInstance.Object->GeneratedClass);
	}

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> FindSkeletalMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/StarterPack/Characters/Mannequin_UE4/Meshes/SK_Mannequin.SK_Mannequin'"));
	if(FindSkeletalMesh.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(FindSkeletalMesh.Object);
		GetMesh()->SetRelativeLocation(FVector(0,0,-96.0f));
		GetMesh()->SetRelativeRotation(FRotator(0,-90,0));
	}
	
	// AIController 클래스를 기본 AIController 클래스로 설정
	AIControllerClass = AEnemyAIController::StaticClass();
}

// Called when the game starts or when spawned
void AEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AEnemyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void AEnemyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

