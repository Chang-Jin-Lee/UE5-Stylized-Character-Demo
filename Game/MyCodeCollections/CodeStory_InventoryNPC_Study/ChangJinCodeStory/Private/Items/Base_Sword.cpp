// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Base_Sword.h"


// Sets default values
ABase_Sword::ABase_Sword()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SwordStaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SwordStaticMeshComponent"));
	SwordStaticMeshComponent->SetupAttachment(Base_ItemSceneComponent);
}

// Called when the game starts or when spawned
void ABase_Sword::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABase_Sword::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

