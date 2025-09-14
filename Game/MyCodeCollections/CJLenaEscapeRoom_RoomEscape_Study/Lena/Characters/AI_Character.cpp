// Fill out your copyright notice in the Description page of Project Settings.


#include "AI_Character.h"

#include "AIController.h"
#include "TimerManager.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/SpotLightComponent.h"
#include "Engine/GameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Lena/Lena.h"
#include "Lena/Controller/ShooterAIController.h"
#include "Lena/GameInstanceSubSystem/LenaEnemyManager.h"
#include "Lena/GameInstanceSubSystem/LenaRoom03Manager.h"
#include "Lena/GameInstanceSubSystem/LenaUIManager.h"
#include "Lena/HUD/Lena_Room03HUD.h"
#include "Lena/UI/Lena_InGameAIHealthBarWidget.h"
#include "Lena/UI/Lena_InGameWIdget.h"

class UBlueprintGeneratedClass;
AAI_Character::AAI_Character()
{
	PrimaryActorTick.bCanEverTick = true;

	HealthBarWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("HealthBarWidget"));
	if(HealthBarWidget)
	{
		HealthBarWidget->SetupAttachment(GetRootComponent());
	}
}

// Called when the game starts or when spawned
void AAI_Character::BeginPlay()
{
	Super::BeginPlay();

	if(g_LenaEnemyManagerIsValid)
	{
		int enemycount = g_LenaEnemyManager->GetEnemyCount();
		g_LenaEnemyManager->SetEnemyCount(enemycount+1);
	}
    
    // AI 캐릭터일 경우 인벤토리에 특정 아이템을 추가
    FInventoryItem AIItem;
    AIItem.ItemName = TEXT("AI Specific Item");
    AIItem.Quantity = 1;
	if(InventoryComponent.IsValid())
	    InventoryComponent->AddItem(AIItem);

	if(UClass* _gun = LoadClass<AGun>(nullptr, TEXT("/Game/BluePrints/Item/BP_M4A1.BP_M4A1_C")))
	{
		GunClassArray.Add(_gun);
	}
    if(!ensure(!GunClassArray.IsEmpty())) return;
	
    USkeletalMeshComponent* SpecificMesh = FindMeshByName(TEXT("BodyMesh"));
    if (SpecificMesh)
    {
    	// 총 부착
    	for(TSubclassOf<AGun> GunClass : GunClassArray)
    	{
    		Gun = GetWorld()->SpawnActor<AGun>(GunClass);
    		if(Gun)
    		{
    			Gun->SetGunSkeletalMeshsimulatePhysics(false);
    			SpecificMesh->HideBoneByName(TEXT("weapon_r"), EPhysBodyOp::PBO_None);
    			Gun->AttachToComponent(SpecificMesh, FAttachmentTransformRules::KeepRelativeTransform, TEXT("WeaponSocket"));
    			Gun->SetOwner(this);
    			Gun->AmmoType = EAmmoType::At_556MM;
    			Gun->SetAmmo(30);
    			// Gun->SetActorHiddenInGame(true);
    			GunArray.Add(Gun);

    			Gun->SetActorRelativeLocation(FVector(3.121f,-5.81f,-2.488f));
    			Gun->SetActorRelativeRotation(FRotator(15.5f,3.611f,15.445f));
    			Gun->SetActorRelativeScale3D(FVector(0.8,0.8,0.8));
    			Gun->SetActorEnableCollision(false);
    			Gun->GetGunSkeletalMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    		}
    	}
    }
	if(GunArray.Num() > WeaponActiveIndex)
	{
		GunArray[WeaponActiveIndex]->SetActorHiddenInGame(false);
		CurrentWeapon = GunArray[WeaponActiveIndex];
	}
	
	FInventoryItem AIInventoryItem;
    AIInventoryItem.ItemID = "Magazine";
    AIInventoryItem.ItemName = "Magazine";
    AIInventoryItem.ItemClass = nullptr;
    AIInventoryItem.ItemImage = nullptr;
    AIInventoryItem.Weight = 0;
    AIInventoryItem.ItemDescription = "";
    AIInventoryItem.Quantity = 60;
    AIInventoryItem.ItemType = EItemType::It_Ammo;
	AIInventoryItem.EquipmentType = EEquipmentType::Et_None;
	AIInventoryItem.GunType = EGunType::Gt_None;
	AIInventoryItem.AmmoType = EAmmoType::At_556MM;

	if(InventoryComponent.IsValid())
		InventoryComponent->Items.Add(AIInventoryItem);
	if (UBlueprintGeneratedClass* _widgetClass = LoadObject<UBlueprintGeneratedClass>(nullptr, TEXT("/Game/UI/HUD/InGame/WBP_InGameAIHealthBarWidget.WBP_InGameAIHealthBarWidget_C")))
	{
		if(	ULena_InGameAIHealthBarWidget* createdwidget = CreateWidget<ULena_InGameAIHealthBarWidget>(GetWorld(), _widgetClass))
		{
			HealthBarWidget->SetWidget(createdwidget);
			HealthBarWidget->SetDrawSize(createdwidget->GetDesiredSize());
			HealthBarWidget->SetWidgetSpace(EWidgetSpace::Screen);
			HealthBarWidget->SetVisibility(false);
		}
	}
}

// Called every frame
void AAI_Character::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if(APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0))
	{
		float Distance = FVector::Dist(GetActorLocation(), PlayerPawn->GetActorLocation());
		if(Distance < AICHARACTER_UI_VISIBLE_DISTANCE)
		{
			if(GetCharacterState() == ECharacterStateType::Cst_Dead)
			{
				if(HealthBarWidget)
				{
					if (HealthBarWidget->IsVisible())
					{
						HealthBarWidget->SetVisibility(false);
						HealthBarWidget->GetWidget()->RemoveFromParent();
						HealthBarWidget = nullptr;
					}
				}
			}
			else if (GetCharacterState() == ECharacterStateType::Cst_Alive)
			{
				if(HealthBarWidget)
				{
					if (HealthBarWidget->IsVisible() == false)
					{
						HealthBarWidget->SetVisibility(true);
					}
					else
					{
						if(ULena_InGameAIHealthBarWidget* widget = Cast<ULena_InGameAIHealthBarWidget>(HealthBarWidget->GetWidget()))
						{
							widget->GetHealth(this);
							widget->GetAmmoInGun(this);
							widget->GetAmmoInInventory(this);
						}
					}
				}
			}
		}
		else
		{
			if(HealthBarWidget)
				if (HealthBarWidget->IsVisible())
					HealthBarWidget->SetVisibility(false);
		}
	}
}

float AAI_Character::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent,
	class AController* EventInstigator, AActor* DamageCauser)
{
	float DamageToApplied =  Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	if(GetCharacterState() == ECharacterStateType::Cst_Dead && IsDeadCounted == false)
	{
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		SpotLightComponent->DestroyComponent();
		PrimaryActorTick.bCanEverTick = false;
		DetachFromControllerPendingDestroy();
		if(AShooterAIController* AIController = Cast<AShooterAIController>(GetController()))
		{
			AIController->StopLogic();
		}
		if(g_LenaEnemyManagerIsValid)
		{
			int32 leftenemy = g_LenaEnemyManager->GetEnemyCount();
			g_LenaEnemyManager->SetEnemyCount(FMath::Max(0,leftenemy-1));
			IsDeadCounted = true;
			if(leftenemy <= 0)
			{
				if(g_LenaUIManaerIsValid)
				{
					InGameWidget = g_LenaUIManager->GetInGameWidget();
					if(InGameWidget)
						InGameWidget->VisibleDescriptionTextRoom03Win();
				}
			}
		}
	}
	return DamageToApplied;
}