#include "ItemSpawnManager.h"
#include "Chaos/PBDRigidClusteringAlgo.h"
#include "Lena/Actor/Items/Base_Item.h"
#include "Lena/Data/ItemSpawnData.h"
#include "Lena/GameInstanceSubSystem/LenaRoom01Manager.h"
#include "Lena/GameInstanceSubSystem/LenaRoom03Manager.h"

struct FLenaRoom03ItemSpawnRow;

AItemSpawnManager::AItemSpawnManager()
{
    PrimaryActorTick.bCanEverTick = true;
}
void AItemSpawnManager::BeginPlay()
{
    Super::BeginPlay();
    SpawnItemsFromTable();
}
void AItemSpawnManager::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}
// 템플릿으로 DataTable에서 스폰된 액터 설정
void AItemSpawnManager::SetItemProperties(TObjectPtr<ABase_Item> SpawnedItem, const FLenaRoom01ItemSpawnRow Row)
{
    if (SpawnedItem)
    {
        SpawnedItem->ItemID = Row.ItemID;
        SpawnedItem->ItemName = Row.ItemName;
        UClass* ActorClass = Cast<UClass>(StaticLoadClass(AActor::StaticClass(), nullptr, *Row.ItemClass));
        if (ActorClass)
        {
            // Set the class for SpawnedItem
            SpawnedItem->ItemClass = ActorClass;
        }
        // UObject* ActorClass = LoadObject<UObject>(nullptr, *Row.ItemClass);
        // if (ActorClass)
        // {
        //     // Set the class for SpawnedItem
        //     SpawnedItem->ItemClass = ActorClass->GetClass();
        // }
        UTexture2D* loadTexture = Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), nullptr, *Row.ItemImage));
        if (loadTexture)
        {
            SpawnedItem->ItemImage = loadTexture;
        }
        SpawnedItem->Weight = Row.Weight;
        SpawnedItem->ItemDescription = Row.ItemDescription;
        SpawnedItem->Quantity = Row.Quantity;

        if(AGun* _equipedgun = Cast<AGun>(SpawnedItem))
        {
            _equipedgun->SetAmmo(Row.CurrentAmmo);
        }
        
        // Set ItemType based on FString
        if (Row.ItemType == "Equipment")
            SpawnedItem->ItemType = EItemType::It_Equipment;
        else if (Row.ItemType == "Consumable")
            SpawnedItem->ItemType = EItemType::It_Consumable;
        else if (Row.ItemType == "Ammo")
            SpawnedItem->ItemType = EItemType::It_Ammo;
        else if (Row.ItemType == "Miscellaneous")
            SpawnedItem->ItemType = EItemType::It_Miscellaneous;
        else
            SpawnedItem->ItemType = EItemType::It_None;

        // Set EquipmentType based on FString
        if (Row.EquipmentType == "Weapon")
            SpawnedItem->EquipmentType = EEquipmentType::Et_Weapon;
        else if (Row.EquipmentType == "Armor")
            SpawnedItem->EquipmentType = EEquipmentType::Et_Armor;
        else if (Row.EquipmentType == "Helmet")
            SpawnedItem->EquipmentType = EEquipmentType::Et_Helmet;
        else if (Row.EquipmentType == "Pants")
            SpawnedItem->EquipmentType = EEquipmentType::Et_Pants;
        else if (Row.EquipmentType == "Shoes")
            SpawnedItem->EquipmentType = EEquipmentType::Et_Shoes;
        else if (Row.EquipmentType == "Shield")
            SpawnedItem->EquipmentType = EEquipmentType::Et_Shield;
        else
            SpawnedItem->EquipmentType = EEquipmentType::Et_None;

        // Set GunType based on FString
        if (Row.GunType == "AssaultRifle")
            SpawnedItem->GunType = EGunType::Gt_AssaultRifle;
        else if (Row.GunType == "SniperRifle")
            SpawnedItem->GunType = EGunType::Gt_SniperRifle;
        else
            SpawnedItem->GunType = EGunType::Gt_None;

        // Set AmmoType based on FString
        if (Row.AmmoType == "5.56mm")
            SpawnedItem->AmmoType = EAmmoType::At_556MM;
        else if (Row.AmmoType == "7.62mm")
            SpawnedItem->AmmoType = EAmmoType::At_762MM;
        else if (Row.AmmoType == "9mm")
            SpawnedItem->AmmoType = EAmmoType::At_9MM;
        else
            SpawnedItem->AmmoType = EAmmoType::At_None;
    }
}

// DataTable에서 아이템을 스폰하는 함수
void AItemSpawnManager::SpawnItemsFromTable()
{
    FString MapName = GetWorld()->GetMapName();
    FString CleanMapName = MapName.Mid(GetWorld()->StreamingLevelsPrefix.Len());

    if(CleanMapName == "Room01_Lab")
    {
        if (g_LenaRoom01ManagerIsValid) // ensure로 안전성 체크
        {
            for (FLenaRoom01ItemSpawnRow Row : g_LenaRoom01Manager->GetLenaRoom01ItemSpawnData())
            {
                SpawnItem(Row);
            }
        }
    }
    else if ( CleanMapName == "Room03_Tokyo")
    {
        if (g_LenaRoom03ManagerIsValid) // ensure로 안전성 체크
        {
            for (FLenaRoom01ItemSpawnRow Row : g_LenaRoom03Manager->GetLenaRoom03ItemSpawnData())
            {
                SpawnItem(Row);
            }
        }
    }
}

void AItemSpawnManager::SpawnItem(FLenaRoom01ItemSpawnRow Row)
{
    FVector3d _position = FVector3d::Zero();
    _position.X = FCString::Atof(*Row.SpawnPoint[0]);
    _position.Y = FCString::Atof(*Row.SpawnPoint[1]);
    _position.Z = FCString::Atof(*Row.SpawnPoint[2]);
    FTransform SpawnTransform(_position);
    if (UClass* ActorClass = Cast<UClass>(StaticLoadObject(UClass::StaticClass(), nullptr, *Row.ItemClass)))
    {
        if (AActor* SpawnedItemActor = GetWorld()->SpawnActor<AActor>(ActorClass, SpawnTransform))
        {
            if (ABase_Item* SpawnedItem = Cast<ABase_Item>(SpawnedItemActor))
            {
                SetItemProperties(SpawnedItem, Row);
            }
        }
    }
}