// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/LongSword.h"


// Sets default values
ALongSword::ALongSword()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	static ConstructorHelpers::FObjectFinder<UStaticMesh> FindLongSwordMesh(TEXT("/Script/Engine.StaticMesh'/Game/Assets/LongSword.LongSword'"));
	if(FindLongSwordMesh.Succeeded())
	{
		SwordStaticMeshComponent->SetStaticMesh(FindLongSwordMesh.Object);
	}
}

// Called when the game starts or when spawned
void ALongSword::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ALongSword::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

