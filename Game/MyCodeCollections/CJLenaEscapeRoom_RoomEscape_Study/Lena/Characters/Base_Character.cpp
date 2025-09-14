// Fill out your copyright notice in the Description page of Project Settings.

#include "Base_Character.h"
#include "Lena/GameMode/LenaGameMode.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkinnedMeshComponent.h"
#include "Components/SpotLightComponent.h"
#include "Lena/Controller/ShooterPlayerController.h"
#include "Lena/Actor/Items/Inventory/InventoryComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Lena/GameInstanceSubSystem/LenaRoom01Manager.h"
#include "Lena/GameInstanceSubSystem/LenaUIManager.h"
#include "Lena/GameMode/LenaRoom03GameMode.h"

// Sets default values
ABase_Character::ABase_Character()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// if(UInventoryComponent* InventoryComp = CreateDefaultSubobject<UInventoryComponent>(TEXT("InventoryComponent")))
	// 	InventoryComponent = InventoryComp;
	// if(UInventoryComponent* InventoryComp = CreateDefaultSubobject<UInventoryComponent>(TEXT("GroundItemsComponent")))
	// 	GroundItemsComponent = InventoryComp;
	// if(UInventoryComponent* InventoryComp = CreateDefaultSubobject<UInventoryComponent>(TEXT("WeaponSlotsComponent")))
	// 	WeaponSlotsComponent = InventoryComp;
	
	InventoryComponentptr = CreateDefaultSubobject<UInventoryComponent>(TEXT("InventoryComponent"));
	GroundItemsComponentptr = CreateDefaultSubobject<UInventoryComponent>(TEXT("GroundItemsComponent"));
	WeaponSlotsComponentptr = CreateDefaultSubobject<UInventoryComponent>(TEXT("WeaponSlotsComponent"));
	SpotLightComponent= CreateDefaultSubobject<USpotLightComponent>(TEXT("SpotLightComponent"));
	if(SpotLightComponent)
		SpotLightComponent->SetupAttachment(GetRootComponent());
		

}

// Called when the game starts or when spawned
void ABase_Character::BeginPlay()
{
	Super::BeginPlay();
	Health = MaxHealth;
	if(AShooterPlayerController* spc = Cast<AShooterPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(),0)))
	{
		CameraManager = spc->PlayerCameraManager;
	}
	CharacterState = ECharacterStateType::Cst_Alive;

	InventoryComponent = InventoryComponentptr;
	GroundItemsComponent = GroundItemsComponentptr;
	WeaponSlotsComponent = WeaponSlotsComponentptr;
	if(WeaponSlotsComponent.IsValid())
		WeaponSlotsComponent->Items.Init(FInventoryItem(), 4);
	// // AI인지 플레이어인지 구분
	// AController* Controller = GetController();
	// if (Controller)
	// {
	// 	if (Controller->IsA<AShooterAIController>())
	// 	{
	// 		UE_LOG(LogTemp, Warning, TEXT("This is an AI Character"));
 //            
	// 		// AI 캐릭터일 경우 인벤토리에 특정 아이템을 추가
	// 		FInventoryItem AIItem;
	// 		AIItem.ItemName = TEXT("AI Specific Item");
	// 		AIItem.Quantity = 1;
	// 		InventoryComponent->AddItem(AIItem);
	// 	}
	// 	else if (Controller->IsA<APlayerController>())
	// 	{
	// 		UE_LOG(LogTemp, Warning, TEXT("This is a Player Character"));
	// 		// 플레이어 캐릭터일 경우 다른 처리를 할 수 있음
	// 	}
	// }

	// if(!ensure(!GunClassArray.IsEmpty())) return;
	//
	// USkeletalMeshComponent* SpecificMesh = FindMeshByName(TEXT("BodyMesh"));
	// if (SpecificMesh)
	// {
	// 	UE_LOG(LogTemp, Warning, TEXT("Find  VRoidCharacter  %s"), *SpecificMesh->GetName());
	// 	
	// 	// 총 부착
	// 	for(TSubclassOf<AGun> GunClass : GunClassArray)
	// 	{
	// 		Gun = GetWorld()->SpawnActor<AGun>(GunClass);
	// 		// SpecificMesh->HideBoneByName(TEXT("weapon_r"), EPhysBodyOp::PBO_None);
	// 		// Gun->AttachToComponent(SpecificMesh, FAttachmentTransformRules::KeepRelativeTransform, TEXT("LHS_ik_hand_r"));
	// 		// Gun->SetOwner(this);
	// 		// Gun->SetActorHiddenInGame(true);
	// 		GunArray.Add(Gun);
	// 		UE_LOG(LogTemp, Warning, TEXT("Spawn Guns  %s"), *Gun->GetName());
	// 	}
	// }
	// GunArray[WeaponActiveIndex]->SetActorHiddenInGame(false);
}

// Called to bind functionality to input
void ABase_Character::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAction(TEXT("Shoot"), EInputEvent::IE_Pressed, this, &ABase_Character::PerformAction);
	PlayerInputComponent->BindAction(TEXT("Reload"), EInputEvent::IE_Pressed, this, &ABase_Character::ReloadAction);
	PlayerInputComponent->BindAction(TEXT("DrawWeapon1"), EInputEvent::IE_Pressed, this, &ABase_Character::ChangeWeapon1);
	PlayerInputComponent->BindAction(TEXT("DrawWeapon2"), EInputEvent::IE_Pressed, this, &ABase_Character::ChangeWeapon2);
	PlayerInputComponent->BindAction(TEXT("ToggleFlashLight"), EInputEvent::IE_Pressed, this, &ABase_Character::ToggleFlashLight);
}

// 특정 이름을 가진 자식 메쉬를 찾는 함수
USkeletalMeshComponent* ABase_Character::FindMeshByName(FName MeshName)
{
	// 모든 SkeletalMeshComponent를 찾습니다.
	TArray<USkeletalMeshComponent*> MeshComponents;
	GetComponents<USkeletalMeshComponent>(MeshComponents);

	// 각 컴포넌트를 순회하며 이름이 일치하는 메쉬를 찾습니다.
	for (USkeletalMeshComponent* MeshComp : MeshComponents)
		if (MeshComp->GetName() == MeshName.ToString())
			return MeshComp;
	// 찾지 못하면 nullptr 반환
	return nullptr;
}

// void ABase_Character::EquipItem(const FInventoryItem& Item, int32 Index)
// {
// 	if(Item.GunType == EGunType::Gt_AssaultRifle || Item.GunType == EGunType::Gt_SniperRifle)
// 	{
// 		WeaponSlotsComponent->Items[Index] = Item;
// 		if(!CurrentWeapon)
// 		{
// 			DrawWeapon(Index);
// 		}
// 	}
// }


void ABase_Character::DrawWeapon(int32 WeaponIndex)
{
	FInventoryItem& Item = WeaponSlotsComponent->Items[WeaponIndex];

	if(!Item.IsEmpty())
	{
		USkeletalMeshComponent* SpecificMesh = FindMeshByName(TEXT("BodyMesh"));
		if(CurrentWeapon)
		{
			CurrentWeapon->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
			// CurrentWeapon->SetActorHiddenInGame(true);
			CurrentWeapon->SetActorEnableCollision(false);
			CurrentWeapon->Destroy();
		}
		
		if(ABase_Weapon* spawneditem = GetWorld()->SpawnActor<ABase_Weapon>(Item.ItemClass))
		{
			CurrentWeapon = spawneditem;
		}
		//CurrentWeapon = GetWorld()->SpawnActor<ABase_Weapon>(Item.ItemClass);
		CurrentWeapon->ItemID = Item.ItemID;
		CurrentWeapon->ItemName = Item.ItemName;
		CurrentWeapon->ItemClass = Item.ItemClass;
		CurrentWeapon->ItemImage = Item.ItemImage;
		CurrentWeapon->Quantity = Item.Quantity;
		CurrentWeapon->Weight = Item.Weight;
		CurrentWeapon->ItemDescription = Item.ItemDescription;
		CurrentWeapon->EquipmentType = Item.EquipmentType;
		CurrentWeapon->ItemType = Item.ItemType;
		CurrentWeapon->AmmoType = Item.AmmoType;
		CurrentWeapon->GunType = Item.GunType;
		
		if (CurrentWeapon)
		{
			if (AGun* _equippedGun = Cast<AGun>(CurrentWeapon.Get()))
			{
				_equippedGun->SetAmmo(Item.CurrentAmmo);
			}
		}
		
		if (CurrentWeapon)
		{
			// 액터에 연결된 모든 컴포넌트를 가져옴
			TArray<UActorComponent*> Components;
			CurrentWeapon->GetComponents(Components);

			// 모든 컴포넌트에 대해 반복하여 충돌을 비활성화
			for (UActorComponent* Component : Components)
			{
				UPrimitiveComponent* PrimitiveComponent = Cast<UPrimitiveComponent>(Component);
				if (PrimitiveComponent)
					PrimitiveComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			}
		}
		SpecificMesh->HideBoneByName(TEXT("weapon_r"), EPhysBodyOp::PBO_None);
		CurrentWeapon->AttachToComponent(SpecificMesh, FAttachmentTransformRules::KeepRelativeTransform, TEXT("WeaponSocket"));
		// CurrentWeapon->SetActorHiddenInGame(true);
		CurrentWeapon->SetOwner(this);

		DrawWeaponEvent();
	}
}

void ABase_Character::PutDownWeapon()
{
	if(CurrentWeapon)
	{
		CurrentWeapon->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		// CurrentWeapon->SetActorHiddenInGame(true);
		CurrentWeapon->SetActorEnableCollision(false);
		CurrentWeapon->Destroy();
		PutDownWeaponEvent();
	}
}


// void ABase_Character::Shoot()
// {
// 	if(IsArmed)
// 	{ 
// 		GunArray[WeaponActiveIndex]->PullTrigger();
// 	}
// }

void ABase_Character::PerformAction()
{
	if(IsArmed && IsAimed && CurrentWeapon)
	{
		if(AGun* CurrentGun = Cast<AGun>(CurrentWeapon.Get()))
		{
			CurrentGun->PullTrigger();
		}
	}
}

void ABase_Character::ReloadAction()
{
	ReloadGun();
}

void ABase_Character::ReloadAIAction()
{
	ReloadEvent();
	ReloadGun();
}

float ABase_Character::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser)
{
	float DamageToApplied = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	DamageToApplied = FMath::Min(Health, DamageToApplied);
	Health -= DamageToApplied;

	if(IsDead())
	{
		if(ALenaRoom03GameMode *GameMode = GetWorld()->GetAuthGameMode<ALenaRoom03GameMode>())
			GameMode->PawnKilled(this);
		DetachFromControllerPendingDestroy();
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		OnDead();
	}
	return DamageToApplied;
}

bool ABase_Character::IsDead()
{
	// 체력이 0보다 작으면 true를 반환.
	if(Health <= 0 && CharacterState != ECharacterStateType::Cst_Dead)
	{
		CharacterState = ECharacterStateType::Cst_Dead;
		return true;
	}
	return false;
}

float ABase_Character::GetHealthPercent() const
{
	return Health / MaxHealth;
}

// void ABase_Character::ChangeWeapon1()
// {
// 	WeaponActiveIndex = 0;
// 	ChangeWeapon();
// }
//
// void ABase_Character::ChangeWeapon2()
// {
// 	WeaponActiveIndex = 1;
// 	ChangeWeapon();
// }

void ABase_Character::ChangeWeapon1()
{
	ChangeWeapon(0);
}

void ABase_Character::ChangeWeapon2()
{
	ChangeWeapon(1);
}

// void ABase_Character::ChangeWeapon()
// {
// 	for(int32 WeaponIndex = 0 ; WeaponIndex < GunArray.Num(); WeaponIndex++)
// 	{
// 		if(WeaponIndex == WeaponActiveIndex)
// 		{
// 			GunArray[WeaponIndex]->SetActorHiddenInGame(false);
// 		}
// 		else
// 		{
// 			GunArray[WeaponIndex]->SetActorHiddenInGame(true);
// 		}
// 	}
// }

// void ABase_Character::HideAllWeapons()
// {
// 	for(int32 WeaponIndex = 0 ; WeaponIndex < GunArray.Num(); WeaponIndex++)
// 	{
// 		GunArray[WeaponIndex]->SetActorHiddenInGame(true);
// 	}
// }

void ABase_Character::ChangeWeapon(int32 Index)
{
	if(WeaponSlotsComponent->Items.Num() > Index)
		if(!WeaponSlotsComponent->Items[Index].IsEmpty())
			DrawWeapon(Index);
}

void ABase_Character::SetOverLayString(FString st)
{
	OverLayString = st;
}

void ABase_Character::AdjustOverLay()
{
	if(OverLayString == "Rifle")
		ToggleIsArmed();
		// ChangeWeapon();
	else
		ToggleIsArmed();
		// HideAllWeapons();
}

bool ABase_Character::GetIsAICharacter()
{
	return IsAICharacter;
}

void ABase_Character::ToggleIsArmed()
{
	IsArmed = !IsArmed;
}

// int ABase_Character::GetAmmoIndex()
// {
// 	return GunArray[WeaponActiveIndex]->GetAmmoIndex();
// }
//
// void ABase_Character::AddAmmo_5mm()
// {
// 	int MaxAmmo = GunArray[WeaponActiveIndex]->GetMaxAmmo();
// 	Ammo_5mm+=MaxAmmo;
// }

bool ABase_Character::CheckAmmo()
{
	if(AGun* Gun = Cast<AGun>(CurrentWeapon))
	{
		if(GetAmmoIndex(Gun->AmmoType) != INDEX_NONE)
			return true;
		else
			return false;
	}
	else
		return false;
}


int32 ABase_Character::GetAmmoIndex(EAmmoType At)
{
	if (InventoryComponent.IsValid() && InventoryComponent->Items.Num() > 0)
	{
		for(int32 i = 0; i < InventoryComponent->Items.Num(); ++i)
			if( InventoryComponent->Items[i].ItemType == EItemType::It_Ammo && InventoryComponent->Items[i].AmmoType == At)
				return i;
	}
	return INDEX_NONE;
}

ABase_Weapon* ABase_Character::GetWeapon()
{
	if(CurrentWeapon)
		return CurrentWeapon.Get();
	else
		return nullptr;
}

bool ABase_Character::ReloadGun()
{
	if(AGun* Gun = Cast<AGun>(CurrentWeapon))
	{
		int32 InventoryIndex = GetAmmoIndex(Gun->AmmoType);
		
		if(InventoryComponent->Items.Num() > InventoryIndex && InventoryComponent->Items[InventoryIndex].Quantity > 0)
		{
			int32 CurrentAmmo = Gun->GetAmmo();
			int32 GunMaxAmmo = Gun->GetMaxAmmo();
			if(CurrentAmmo < GunMaxAmmo)
			{
				Gun->SetAmmo(FMath::Min(GunMaxAmmo, CurrentAmmo+InventoryComponent->Items[InventoryIndex].Quantity));
				InventoryComponent->Items[InventoryIndex].Quantity = FMath::Max(0,
					InventoryComponent->Items[InventoryIndex].Quantity
					- GunMaxAmmo
					+ CurrentAmmo);
				if(InventoryComponent->Items[InventoryIndex].Quantity <= 0)
					InventoryComponent->RemoveItem(InventoryComponent->Items[InventoryIndex]);
			}
			return true;
		}
	}
	return false;
}


// AGun* ABase_Character::GetWeapon()
// {
// 	return GunArray[WeaponActiveIndex];
// }
//
// bool ABase_Character::ReloadGun()
// {
// 	int MaxAmmo = GunArray[WeaponActiveIndex]->GetMaxAmmo();
// 	int CurrentAmmo = GunArray[WeaponActiveIndex]->GetAmmoIndex();
//
// 	if(!GunArray[WeaponActiveIndex])
// 		return false;
//
// 	if(CurrentAmmo < MaxAmmo)
// 	{
// 		GunArray[WeaponActiveIndex]->SetAmmo(FMath::Min(MaxAmmo, CurrentAmmo+Ammo_5mm));
// 		Ammo_5mm = FMath::Max( 0 , Ammo_5mm-(MaxAmmo - CurrentAmmo));
// 	}
// 	
// 	return true;
// }

// -----------------------------
// Inventory
// -----------------------------

// 아이템 떨어뜨리기
void ABase_Character::DropItem(FInventoryItem ItemData, EInventorySlotType InventorySlot)
{
	// ItemActor에서 ItemID를 가져옵니다. ItemActor는 아이템 정보를 제공해야 합니다.
	if(ABase_Item* Item = Cast<ABase_Item>(ItemData.ItemActor))
	{
		float Rad = 20.0f; // 반경 설정
		float InnerRad = 5.0f; // 제외할 범위 설정

		// 랜덤한 값을 선택하는 함수
		auto GetRandomOffset = [Rad, InnerRad]() -> float
		{
			float Offset;
			do
			{
				Offset = FMath::RandRange(-Rad, Rad);
			} while (Offset > -InnerRad && Offset < InnerRad);
			return Offset;
		};

		float RandomX = GetRandomOffset();
		float RandomY = GetRandomOffset();
		FVector Location = GetActorLocation() + (RandomX, RandomY, 0.0f);
		FActorSpawnParameters Parameters;
		ABase_Item* SpawnedItem =  GetWorld()->SpawnActor<ABase_Item>(Item->GetClass(), Location, GetActorRotation(), Parameters);
		if(SpawnedItem)
		{
			SpawnedItem->ItemID = ItemData.ItemID;
			SpawnedItem->ItemName = ItemData.ItemName;
			SpawnedItem->ItemClass = ItemData.ItemClass;
			SpawnedItem->ItemImage = ItemData.ItemImage;
			SpawnedItem->Quantity = ItemData.Quantity;
			SpawnedItem->Weight = ItemData.Weight;
			SpawnedItem->ItemDescription = ItemData.ItemDescription;
			SpawnedItem->EquipmentType = ItemData.EquipmentType;
			SpawnedItem->ItemType = ItemData.ItemType;
			SpawnedItem->AmmoType = ItemData.AmmoType;
			SpawnedItem->GunType = ItemData.GunType;
			if (DropItemSound)
			{
				UGameplayStatics::PlaySoundAtLocation(GetWorld(), DropItemSound.Get(), GetActorLocation());
			}
		}
		if(AGun* spawnedgun = Cast<AGun>(SpawnedItem))
		{
			if (CurrentWeapon)
			{
				if(AGun* _equipedgun = Cast<AGun>(CurrentWeapon.Get()))
				{
					spawnedgun->SetAmmo(_equipedgun->GetAmmo());
				}
			}
		}

		if(InventorySlot == EInventorySlotType::ISt_Inventory)
		{
			if(InventoryComponent.IsValid())
				InventoryComponent->RemoveItem(ItemData);
		}
		else if (InventorySlot == EInventorySlotType::ISt_Equipment)
		{
			if(WeaponSlotsComponent.IsValid())
			{
				WeaponSlotsComponent->ResetItem(ItemData);
				PutDownWeapon();
				if(CurrentWeapon)
				{
					CurrentWeapon->Destroy();
					CurrentWeapon = nullptr;
				}
			}
		}
	}
}

// 아이템 줍기
void ABase_Character::PickupItem(AActor* ItemActor, EInventorySlotType InventorySlot)
{
	if (ItemActor)
	{
		// ItemActor에서 ItemID를 가져옵니다. ItemActor는 아이템 정보를 제공해야 합니다.
		if(ABase_Item* Item = Cast<ABase_Item>(ItemActor))
		{
			FInventoryItem InventoryItem;
			InventoryItem.ItemID = Item->ItemID;
			InventoryItem.ItemName = Item->ItemName;
			InventoryItem.ItemClass = Item->ItemClass;
			InventoryItem.ItemImage = Item->ItemImage;
			InventoryItem.Quantity = Item->Quantity;
			InventoryItem.Weight = Item->Weight;
			InventoryItem.ItemDescription = Item->ItemDescription;
			InventoryItem.ItemActor = Item;
			InventoryItem.EquipmentType = Item->EquipmentType;
			InventoryItem.ItemType = Item->ItemType;
			InventoryItem.AmmoType = Item->AmmoType;
			InventoryItem.GunType = Item->GunType;
			if(AGun* _equipedgun = Cast<AGun>(Item))
			{
				InventoryItem.CurrentAmmo = _equipedgun->GetAmmo();
			}

			if(InventorySlot == EInventorySlotType::ISt_Ground)
			{
				if (InventoryComponent.IsValid())
					InventoryComponent->AddItem(InventoryItem);
				if(GroundItemsComponent.IsValid())
					GroundItemsComponent->RemoveItem(InventoryItem);
			}
			else if (InventorySlot == EInventorySlotType::ISt_Equipment)
			{
				if (InventoryComponent.IsValid())
					InventoryComponent->AddItem(InventoryItem);
				
				if(WeaponSlotsComponent.IsValid())
				{
					WeaponSlotsComponent->ResetItem(InventoryItem);
					PutDownWeapon();
					if(CurrentWeapon)
					{
						CurrentWeapon->Destroy();
						CurrentWeapon = nullptr;
					}
				}
			}

			if(PickupItemSound)
				UGameplayStatics::PlaySoundAtLocation(GetWorld(), PickupItemSound.Get(), GetActorLocation());
			// 아이템 액터를 월드에서 제거합니다.
			ItemActor->Destroy();
		}
	}
}

// 아이템 장착
void ABase_Character::EquipItem(FInventoryItem ItemData, EInventorySlotType InventorySlot, int32 SlotIndex)
{
	if(InventorySlot == EInventorySlotType::ISt_Inventory)
	{
		if(InventoryComponent.IsValid())
			InventoryComponent->RemoveItem(ItemData);
		if(WeaponSlotsComponent.IsValid() && WeaponSlotsComponent->Items.Num() > SlotIndex)
		{
			WeaponSlotsComponent->Items[SlotIndex] = ItemData;
			if(!CurrentWeapon)
				DrawWeapon(SlotIndex);
		}
	}
	else if (InventorySlot == EInventorySlotType::ISt_Ground)
	{
		if(GroundItemsComponent.IsValid())
			GroundItemsComponent->RemoveItem(ItemData);
		if(WeaponSlotsComponent.IsValid() && WeaponSlotsComponent->Items.Num() > SlotIndex)
		{
			WeaponSlotsComponent->Items[SlotIndex] = ItemData;
			if(!CurrentWeapon)
				DrawWeapon(SlotIndex);
		}
		if(ABase_Item* Item = Cast<ABase_Item>(ItemData.ItemActor))
			Item->Destroy();
	}
	
	if(PickupItemSound)
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), PickupItemSound.Get(), GetActorLocation());
}

void ABase_Character::CheckGroundItem(AActor* ItemActor)
{
	if (ItemActor)
	{
		ABase_Item* Item = Cast<ABase_Item>(ItemActor);
		
		if(Item)
		{
			FInventoryItem InventoryItem;
			InventoryItem.ItemID = Item->ItemID;
			InventoryItem.ItemName = Item->ItemName;
			InventoryItem.ItemClass = Item->ItemClass;
			InventoryItem.ItemImage = Item->ItemImage;
			InventoryItem.Quantity = Item->Quantity;
			InventoryItem.Weight = Item->Weight;
			InventoryItem.ItemDescription = Item->ItemDescription;
			InventoryItem.ItemActor = Item;
			InventoryItem.EquipmentType = Item->EquipmentType;
			InventoryItem.ItemType = Item->ItemType;
			InventoryItem.AmmoType = Item->AmmoType;
			InventoryItem.GunType = Item->GunType;
			if(AGun* _gun = Cast<AGun>(Item))
			{
				InventoryItem.CurrentAmmo = _gun->GetAmmo();
			}
			if (GroundItemsComponent.IsValid())
				GroundItemsComponent->AddItem(InventoryItem);
		}
	}
}

void ABase_Character::ToggleFlashLight()
{
	SpotLightComponent->IsVisible() == true ? SpotLightComponent->SetVisibility(false) : SpotLightComponent->SetVisibility(true);
}