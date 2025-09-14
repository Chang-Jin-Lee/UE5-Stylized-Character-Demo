// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Katana.h"


// Sets default values
AKatana::AKatana()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> FindKatanaMesh(TEXT("/Game/Assets/KatanaMesh.KatanaMesh"));
	if(FindKatanaMesh.Succeeded())
	{
		SwordStaticMeshComponent->SetStaticMesh(FindKatanaMesh.Object);
	}
}

// Called when the game starts or when spawned
void AKatana::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AKatana::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

