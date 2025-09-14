// Fill out your copyright notice in the Description page of Project Settings.

#include "Lena/Actor/Items/Gun.h"
#include "Components/SkeletalMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h" 
#include "GenericPlatform/GenericPlatformCrashContext.h"
#include "Engine/DamageEvents.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "Components/SceneCaptureComponent2D.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/World.h"
#include "GameFramework/Pawn.h"
#include "UObject/ConstructorHelpers.h"

// Sets default values
AGun::AGun()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	GunSkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	if(GunSkeletalMesh)
	{
		GunSkeletalMesh->SetupAttachment(Root);
	}
	MagazineMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MagazineMesh"));
	if(MagazineMesh)
	{
		MagazineMesh->SetupAttachment(GunSkeletalMesh);
	}

	static ConstructorHelpers::FObjectFinder<USoundBase> MuzzleSoundFind(TEXT("/Game/SFX/A_RifleShot_Que.A_RifleShot_Que"));
	if(MuzzleSoundFind.Succeeded())
	{
		MuzzleSound = MuzzleSoundFind.Object;
	}

	static ConstructorHelpers::FObjectFinder<USoundBase> ImpactSoundFind(TEXT("/Game/SFX/A_RifleImpact_Que.A_RifleImpact_Que"));
	if(ImpactSoundFind.Succeeded())
	{
		ImpactSound = ImpactSoundFind.Object;
	}

	static ConstructorHelpers::FObjectFinder<USoundBase> AmmoEmptySoundFind(TEXT("/Script/Engine.SoundWave'/Game/SFX/Audio/Gun/A_Gun_Empty.A_Gun_Empty'"));
	if(AmmoEmptySoundFind.Succeeded())
	{
		AmmoEmptySound = AmmoEmptySoundFind.Object;
	}

	static ConstructorHelpers::FObjectFinder<UNiagaraSystem> MuzzleFlasEffectFind(TEXT("/Script/Niagara.NiagaraSystem'/Game/AssetPacks/MuzzleFlash3D/FX/Modern/NS_MuzzleFlash_6.NS_MuzzleFlash_6'"));
	if(MuzzleFlasEffectFind.Succeeded())
	{
		MuzzleFlashNS = MuzzleFlasEffectFind.Object;
	}

	static ConstructorHelpers::FObjectFinder<UParticleSystem> ImpactEffectFind(TEXT("/Script/Engine.ParticleSystem'/Game/AssetPacks/ShooterGame/Effects/ParticleSystems/Weapons/AssaultRifle/Impacts/P_AssaultRifle_IH.P_AssaultRifle_IH'"));
	if(ImpactEffectFind.Succeeded())
	{
		ImpactEffect = ImpactEffectFind.Object;
	}

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> FindMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/Props/Meshes/M4A1.M4A1'"));
	if(FindMesh.Succeeded())
	{
		GunSkeletalMesh->SetSkeletalMesh(FindMesh.Object);
		// Mesh가 USkeletalMeshComponent라고 가정
		GunSkeletalMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics); // 물리와 쿼리 충돌 활성화
		GunSkeletalMesh->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic); // 필요에 맞게 오브젝트 타입 설정
		// 충돌 프리셋 설정 (모든 채널 기본적으로 블록)
		GunSkeletalMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
		// Pawn 채널은 무시하도록 설정
		GunSkeletalMesh->SetCollisionResponseToChannel(ECC_Pawn, ECollisionResponse::ECR_Ignore);
	}
	static ConstructorHelpers::FObjectFinder<UStaticMesh> FindMagazineMesh(TEXT("/Script/Engine.StaticMesh'/Game/Props/Meshes/Magazine.Magazine'"));
	if(FindMagazineMesh.Succeeded())
	{
		MagazineMesh->SetStaticMesh(FindMagazineMesh.Object);
		MagazineMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}

void AGun::PullTrigger()
{
	if(IsEmpty())
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), AmmoEmptySound, GetActorLocation());
		return;
	}
	if (MuzzleFlashNS)
	{
		UNiagaraFunctionLibrary::SpawnSystemAttached(
			MuzzleFlashNS,
			GunSkeletalMesh,
			TEXT("MuzzleFlashSocket"),
			FVector::ZeroVector,
			FRotator::ZeroRotator,
			EAttachLocation::SnapToTargetIncludingScale,
			true
		);
	}
	UGameplayStatics::SpawnSoundAttached(MuzzleSound, GunSkeletalMesh, TEXT("MuzzleFlashSocket"));
	
	FHitResult HitResult;
	FVector ShotDirection;

	bool bSuccess = GunTrace(HitResult, ShotDirection);
	if(bSuccess)
	{
		// DrawDebugPoint(GetWorld(), HitResult.Location, 15, FColor::Red, false, 5);
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactEffect, HitResult.Location, ShotDirection.Rotation());
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), ImpactSound, HitResult.Location);

		AActor* DamagedActor = HitResult.GetActor();
		if(DamagedActor)
		{
			FPointDamageEvent DamageEvent(Damage, HitResult, ShotDirection, nullptr);
			AController *OwnerController = GetOwnerController(); // 두번 계산하더라도, 코드의 가독성이 좋아지고 동기화가 어긋날 수 있는 변수나 상태를 저장하지 않아도 된다
			DamagedActor->TakeDamage(Damage, DamageEvent, OwnerController, this);
		}
	}
	Ammo = FMath::Max(0, Ammo-1);
}

// Called when the game starts or when spawned
void AGun::BeginPlay()
{
	Super::BeginPlay();
	GunSkeletalMesh->SetSimulatePhysics(true);
	FAttachmentTransformRules Rules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::KeepRelative, true);
	MagazineMesh->AttachToComponent(GunSkeletalMesh, Rules, FName(TEXT("mag")));
	GunSkeletalMesh->HideBoneByName(FName(TEXT("magazine")), PBO_None);
}

// Called every frame
void AGun::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AGun::SetGunSkeletalMeshsimulatePhysics(bool simulatePhysics)
{
	GunSkeletalMesh->SetSimulatePhysics(simulatePhysics);
}

bool AGun::GunTrace(FHitResult& HitResult, FVector& ShotDirection)
{
	AController* OwnerController =  GetOwnerController();
	if(OwnerController == nullptr)
		return false;
	
	FVector Location;
	FRotator Rotation;
	OwnerController->GetPlayerViewPoint(Location,Rotation);
	ShotDirection = -Rotation.Vector();
	// 끝점을 위한 FVector
	FVector End = Location + Rotation.Vector() * MaxRange;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);
	Params.AddIgnoredActor(GetOwner());
	return GetWorld()->LineTraceSingleByChannel(HitResult, Location, End, ECollisionChannel::ECC_GameTraceChannel1,Params);
}

TObjectPtr<AController> AGun::GetOwnerController() const
{
	if(APawn* OwnerPawn = Cast<APawn>(GetOwner()))
		return OwnerPawn->GetController();
	else
		return nullptr;
}

void AGun::SetAmmo(int AmmoCount)
{
	Ammo = AmmoCount;
}

int AGun::GetAmmo()
{
	return Ammo;
}

bool AGun::IsEmpty()
{
	return Ammo <= 0;
}

int AGun::GetMaxAmmo()
{
	return MaxAmmo;
}
