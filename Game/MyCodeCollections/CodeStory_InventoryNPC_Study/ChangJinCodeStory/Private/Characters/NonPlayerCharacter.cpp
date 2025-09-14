// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/NonPlayerCharacter.h"

#include "Components/CapsuleComponent.h"
#include "Components/InventoryComponent.h"
#include "Items/Katana.h"
#include "Items/LongSword.h"


// Sets default values
ANonPlayerCharacter::ANonPlayerCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComponent"));
	CapsuleComponent->SetupAttachment(RootComponent);
	CapsuleComponent->SetCapsuleRadius(GetCapsuleComponent()->GetUnscaledCapsuleRadius() * 1.2f);
	CapsuleComponent->SetCapsuleHalfHeight(GetCapsuleComponent()->GetUnscaledCapsuleHalfHeight() * 1.2f);

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> FindSkeletalMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/StarterPack/Characters/Mannequins/Meshes/SKM_Quinn.SKM_Quinn'"));
	if(FindSkeletalMesh.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(FindSkeletalMesh.Object);
		GetMesh()->SetRelativeLocation(FVector(0,0,-96.0f));
		GetMesh()->SetRelativeRotation(FRotator(0,-90,0));
	}

	InventoryWidgetPosition = FVector2D(350, 250);
	InventoryGridCount = FVector2D(4,4);

	if(InventoryComponent)
	{
		InventoryComponent->Items.SetNum(InventoryGridCount.X * InventoryGridCount.Y);
		for(int32 i = 0; i < InventoryGridCount.X * InventoryGridCount.Y; i++)
		{
			InventoryComponent->Items[i].Empty();
		}
		InventoryComponent->Items[0].TestSword1();
		InventoryComponent->Items[1].TestSword2();
	
		static ConstructorHelpers::FObjectFinder<UTexture2D> FindSword1(TEXT("/Script/Engine.Texture2D'/Game/Image/Sword1.Sword1'"));
		if (FindSword1.Succeeded())
		{
			InventoryComponent->Items[0].ItemImage = FindSword1.Object;
		}

		static ConstructorHelpers::FObjectFinder<UTexture2D> FindSword2(TEXT("/Script/Engine.Texture2D'/Game/Image/Sword2.Sword2'"));
		if (FindSword2.Succeeded())
		{
			InventoryComponent->Items[1].ItemImage = FindSword2.Object;
		}
	}
	
}

// Called when the game starts or when spawned
void ANonPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ANonPlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void ANonPlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void ANonPlayerCharacter::OpenNpcInventory()
{
	OpenInventory();
}

