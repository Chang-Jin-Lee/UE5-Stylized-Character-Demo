// Copyright ChangJin


#include "Actor/Weapon/AuraWeaponBase.h"

// Sets default values
AAuraWeaponBase::AAuraWeaponBase()
{
	PrimaryActorTick.bCanEverTick = true;

	WeaponSkeletalMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponSkeletalMeshComponent"));
	check(WeaponSkeletalMeshComponent);
	WeaponSkeletalMeshComponent->SetupAttachment(RootComponent);
	
}

void AAuraWeaponBase::HighlightActor()
{
	bHightlighted = true;
}

void AAuraWeaponBase::UnHighlightActor()
{
	bHightlighted = false;
}

void AAuraWeaponBase::BeginPlay()
{
	Super::BeginPlay();
}

void AAuraWeaponBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(bHightlighted)
	{
		WeaponSkeletalMeshComponent->SetRenderCustomDepth(true);
		WeaponSkeletalMeshComponent->SetCustomDepthStencilValue(250);
	}
	else
	{
		WeaponSkeletalMeshComponent->SetRenderCustomDepth(false);
	}
}

