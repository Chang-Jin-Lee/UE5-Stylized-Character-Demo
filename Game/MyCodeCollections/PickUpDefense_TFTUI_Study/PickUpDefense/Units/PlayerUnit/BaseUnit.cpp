// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseUnit.h"
#include "UnitStats.h"
#include "Blueprint/UserWidget.h"
#include "Components/SphereComponent.h"
#include "Engine/DataTable.h"
#include "Engine/SkeletalMesh.h"
#include "Kismet/GameplayStatics.h"
#include "PickUpDefense/Units/Enemy/BaseEnemy.h"
#include "PickUpDefense/UI/OptionMenu.h"
#include "PickUpDefense/Units/Animation/AnimData.h"
#include "PickUpDefense/Units/Animation/CharacterAnimInstance.h"

// Sets default values
ABaseUnit::ABaseUnit()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	/* Scene Component */
	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	RootComponent = SceneComponent;
	
	/* Sphere Collider */
	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere Collider"));
	SphereComponent->SetupAttachment(SceneComponent);
	SphereComponent->SetSphereRadius(AttackRange);

	/* Static Mesh */
	BaseSkeletalMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("BaseSkeletalMeshComponent"));
	BaseSkeletalMeshComponent->SetupAttachment(SphereComponent);

	// static ConstructorHelpers::FObjectFinder<USkeletalMesh> SphereMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/Assets/Ahri/Ahri_gltf.Ahri_gltf'"));
	// if (SphereMesh.Succeeded())
	// {
	// 	BaseSkeletalMeshComponent->SetSkeletalMesh(SphereMesh.Object);
	// 	BaseSkeletalMeshComponent->SetRelativeScale3D(FVector3d(0.04,0.04,0.04));
	// }

	if(BaseSkeletalMeshComponent)
	{
		// // 애니메이션 블루프린트를 로드합니다.
		// static ConstructorHelpers::FClassFinder<UCharacterAnimInstance> AnimBPClass(TEXT("/Script/Engine.AnimBlueprint'/Game/Characters/ABP_Ahri.ABP_Ahri'"));
  //   
		// if (AnimBPClass.Succeeded())
		// {
		// 	// 애니메이션 블루프린트를 설정합니다.
		// 	BaseSkeletalMeshComponent->SetAnimInstanceClass(AnimBPClass.Class);
		// }
		// else
		// {
		// 	BaseSkeletalMeshComponent->SetAnimInstanceClass(UCharacterAnimInstance::StaticClass());
		// 	UE_LOG(LogTemp, Warning, TEXT("Failed to load AnimBlueprint class"));
		// }
		
		// BaseSkeletalMeshComponent->SetAnimInstanceClass(UCharacterAnimInstance::StaticClass());
		
		BaseSkeletalMeshComponent->SetCollisionProfileName(TEXT("BlockAll")); // Set a proper collision profile
		BaseSkeletalMeshComponent->SetGenerateOverlapEvents(true);
		BaseSkeletalMeshComponent->SetNotifyRigidBodyCollision(true);
		BaseSkeletalMeshComponent->OnClicked.AddDynamic(this, &ABaseUnit::OnMeshClicked);
	}
}

void ABaseUnit::OnMeshClicked(UPrimitiveComponent* ClickedComp, FKey ButtonPressed)
{
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);
	if (PlayerController)
	{
		FHitResult HitResult;
		PlayerController->GetHitResultUnderCursorByChannel(TraceTypeQuery1, true, HitResult);

		if (HitResult.GetActor() == this)
		{
			UOptionMenu* OptionMenuWidget = CreateWidget<UOptionMenu>(PlayerController, UOptionMenu::StaticClass());
			if(OptionMenuWidget)
			{
				OptionMenuWidget->TargetUnit = this;
				OptionMenuWidget->AddToViewport();
				// Get the current mouse position
				float MouseX, MouseY;
				PlayerController->GetMousePosition(MouseX, MouseY);

				// Offset values for the widget position (e.g., 10 units to the right and 10 units down)
				FVector2D Offset(10.0f, 10.0f);

				// Set the widget position with the offset
				FVector2D WidgetPosition = FVector2D(MouseX, MouseY) + Offset;
				OptionMenuWidget->SetPositionInViewport(WidgetPosition, true);
			}
		}
	}
}


void ABaseUnit::InitializeUnit(const EUnitAttributeType InType, const int32 InLevel)
{
	UnitAttributeType = InType;
	CurrentLevel = InLevel;

	MaxUpgradeLevel = UnitStats.GetMaxUpgradeLevel(UnitAttributeType);
	
	if(UnitStats.UnitTypeLevelUpgradeStats.Contains(UnitAttributeType))
	{
		if(const TMap<int32, FBaseUnitStats>& BaseStats = UnitStats.UnitTypeLevelUpgradeStats[UnitAttributeType]; BaseStats.Contains(CurrentLevel))
		{
			AttackSpeed = BaseStats[CurrentLevel].AttackSpeed;
			AttackPhysicalPower = BaseStats[CurrentLevel].AttackPhysicalPower; 
			AttackMagicalPower = BaseStats[CurrentLevel].AttackMagicalPower; 
			AttackRange = BaseStats[CurrentLevel].AttackRange; 
			CriticalChance = BaseStats[CurrentLevel].CriticalChance; 
			MissChance = BaseStats[CurrentLevel].MissChance; 
			CriticalMultiplier = BaseStats[CurrentLevel].CriticalMultiplier; 
			
			BuyPrice = BaseStats[CurrentLevel].BuyPrice;
			UpgradeCost = BaseStats[CurrentLevel].UpgradeCost;

			SellPrice = BuyPrice;
		}
	}
	
	/* Set Mesh */
	if(const FUnitMeshData* MeshData = UnitStats.GetMeshData(UnitAttributeType))
	{
		USkeletalMesh* SkeletalMesh = LoadObject<USkeletalMesh>(nullptr, *MeshData->UnitSkeletalMeshDir.ToString());
		if(SkeletalMesh)
		{
			if(BaseSkeletalMeshComponent)
			{
				BaseSkeletalMeshComponent->SetSkeletalMesh(SkeletalMesh);
				BaseSkeletalMeshComponent->SetRelativeScale3D(MeshData->MeshScale * 0.01);
				
				UClass* AnimBPClass = LoadObject<UClass>(nullptr, *MeshData->AnimInstanceDir.ToString());
				if (AnimBPClass)
				{
					// 애니메이션 블루프린트를 설정
					BaseSkeletalMeshComponent->SetAnimInstanceClass(AnimBPClass);
				}
			}
		}
	}

	// 데이터 테이블에서 애니메이션 데이터를 읽어옵니다.
	UDataTable* AnimDataTable = LoadObject<UDataTable>(nullptr, TEXT("/Script/Engine.DataTable'/Game/DataTable/Anim/AnimData.AnimData'"));
	if (!AnimDataTable) return;

	FString EnumString = UEnum::GetDisplayValueAsText(UnitAttributeType).ToString().Replace(TEXT(" "), TEXT(""));
	FName RowName(*EnumString);
	FAnimData* AnimData = AnimDataTable->FindRow<FAnimData>(RowName, TEXT("Unit Animation Context"));
	
	if (AnimData)
	{
		// AnimInstance가 설정된 상태에서 각 애니메이션을 설정합니다.
		if (UCharacterAnimInstance* AnimInstance = Cast<UCharacterAnimInstance>(BaseSkeletalMeshComponent->GetAnimInstance()))
		{
			AnimInstance->WalkAnimation = AnimData->WalkAnim;
			AnimInstance->IdleAnimation = AnimData->IdleAnim;
			AnimInstance->PlayIdleAnimation();
		}
	}
	
	/* Set Sphere Collider */
	SphereComponent->InitSphereRadius(AttackRange);
	SphereComponent->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	SphereComponent->SetRelativeLocation(FVector(0.f, 0.f, 0.f));

	SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &ABaseUnit::OnOverlapBegin);
	SphereComponent->OnComponentEndOverlap.AddDynamic(this, &ABaseUnit::OnOverlapEnd);
}


bool ABaseUnit::CanUpgrade() const
{
	if(CurrentLevel >= MaxUpgradeLevel)
	{
		return false;
	}
	return true;
}


void ABaseUnit::UpgradeUnit()
{
	if(CanUpgrade())
	{
		// 판매 가격 = 구매 가격 + 업그레이드 비용 - 1
		SellPrice += UpgradeCost - 1;

		CurrentLevel++;
		
		SetStats();
	}
}


void ABaseUnit::SetStats()
{
	if(UnitStats.UnitTypeLevelUpgradeStats.Contains(UnitAttributeType))
	{
		if(const TMap<int32, FBaseUnitStats>& BaseStats = UnitStats.UnitTypeLevelUpgradeStats[UnitAttributeType]; BaseStats.Contains(CurrentLevel))
		{
			AttackSpeed = BaseStats[CurrentLevel].AttackSpeed;
			AttackPhysicalPower = BaseStats[CurrentLevel].AttackPhysicalPower; 
			AttackMagicalPower = BaseStats[CurrentLevel].AttackMagicalPower; 
			AttackRange = BaseStats[CurrentLevel].AttackRange; 
			UpgradeCost = BaseStats[CurrentLevel].UpgradeCost;
		}
	}
}


void ABaseUnit::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(OtherActor && OtherActor != this)
	{
		if(ABaseEnemy* OverlappedEnemy = Cast<ABaseEnemy>(OtherActor))
		{	
			if(EnemyAttributeType == OverlappedEnemy->GetAttributeType())
			{
				if(!EnemiesInRage.Contains(OverlappedEnemy))
				{
					EnemiesInRage.Add(OverlappedEnemy);
					EnemiesInQueue.Enqueue(OverlappedEnemy);

					OverlappedEnemy->OnTargetEnemyDestroy.AddDynamic(this, &ABaseUnit::HandleEnemyDestroyed);
				}
				
				if(!bIsAttacking && !TargetEnemy)
				{
					TargetEnemy = OverlappedEnemy;
					StartAttack();
					bIsAttacking = true;
				}
			}
		}
	}
}


void ABaseUnit::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex)
{
	if(OtherActor && OtherActor != this)
	{
		if(const ABaseEnemy* OverlappedEnemy = Cast<ABaseEnemy>(OtherActor))
		{
			if(EnemyAttributeType == OverlappedEnemy->GetAttributeType())
			{
				EnemiesInRage.Remove(OverlappedEnemy);

				if(OverlappedEnemy == TargetEnemy)
				{
					StopAttack();
					TargetEnemy = nullptr;
					
					if(ABaseEnemy* NewTarget = FindTarget())
					{
						TargetEnemy = NewTarget;
						StartAttack();
					}
				}
			}
		}
	}
}


void ABaseUnit::HandleEnemyDestroyed(const ABaseEnemy* DestroyedEnemy)
{
	EnemiesInRage.Remove(DestroyedEnemy);	
}


ABaseEnemy* ABaseUnit::FindTarget()
{
	ABaseEnemy* Target = nullptr;
	
	while(!EnemiesInQueue.IsEmpty())
	{
		EnemiesInQueue.Dequeue(Target);
		
		if(EnemiesInRage.Contains(Target))
		{
			if(Target && !Target->IsPendingKillPending())
			{
				return Target;
			}
		}
	}
	return nullptr;
}


void ABaseUnit::Attack()
{
	/* Attack Sound */

	/* Attack Animation */


	UE_LOG(LogTemp, Warning, TEXT("Attack! %s"),  *GetActorLabel());
	
	if(TargetEnemy && !TargetEnemy->IsPendingKillPending())
	{
		ApplyDamage();
	}
	else
	{
		StopAttack();

		if(ABaseEnemy* NewTarget = FindTarget())
		{
			TargetEnemy = NewTarget;

			RandomSeed = NewTarget->GetUniqueID();
			RandomStream.Initialize(RandomSeed);
			
			StartAttack();
		}
	}
}


void ABaseUnit::ApplyDamage() const
{
	float PhysicalDamage;
	float MagicalDamage;
	
	float RandomValue = RandomStream.FRand();

	if(RandomValue < MissChance)		// 타격 실패
	{
		PhysicalDamage = 0.f;
		MagicalDamage = 0.f;
	}
	else	// 타격 성공
	{
		RandomValue = RandomStream.FRand();

		PhysicalDamage = AttackPhysicalPower;
		MagicalDamage = AttackPhysicalPower;
		
		if(RandomValue < CriticalChance)	// 크리티컬
		{
			PhysicalDamage *= CriticalMultiplier;
			MagicalDamage *= CriticalMultiplier;
		}
	}
	
	TargetEnemy->ApplyDamage(PhysicalDamage, MagicalDamage);
}


void ABaseUnit::StartAttack()
{
	if(!bIsAttacking && TargetEnemy)
	{
		bIsAttacking = true;

		GetWorld()->GetTimerManager().SetTimer(AttackTimerHandle, this, &ABaseUnit::Attack, AttackSpeed, true);
	}
}


void ABaseUnit::StopAttack()
{
	bIsAttacking = false;

	GetWorld()->GetTimerManager().ClearTimer(AttackTimerHandle);
}
