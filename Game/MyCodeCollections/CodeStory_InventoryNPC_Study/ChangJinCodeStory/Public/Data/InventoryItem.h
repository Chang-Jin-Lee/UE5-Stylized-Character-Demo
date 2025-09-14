#pragma once

#include "CoreMinimal.h"
#include "InventoryItem.generated.h"

struct FItemSlotSize
{
	// 슬롯 크기를 정의합니다.
	static constexpr float Width = 100.f;
	static constexpr float Height = 100.f;
};

UENUM(BlueprintType)
enum class EItemType : uint8
{
	It_None UMETA(DisplayName = "None"),
	It_Weapon UMETA(DisplayName = "Weapon"),
	It_Consumable UMETA(DisplayName = "Consumable")
};

UENUM(BlueprintType)
enum class EWeaponType : uint8
{
	Wt_None UMETA(DisplayName = "None"),
	Wt_LongSword UMETA(DisplayName = "LongSword"),
	Wt_Katana UMETA(DisplayName = "Katana"),
	Wt_Shield UMETA(DisplayName = "Shield")
};

USTRUCT(BlueprintType)
struct FInventoryItem
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Item")
	FString ItemID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Item")
	FString ItemName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Item")
	FString ItemDescription;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Item")
	TSubclassOf<AActor> ItemClass; // 아이템 클래스

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Item")
	UTexture2D* ItemImage; // 아이템 이미지

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Item")
	int32 Quantity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Item")
	EItemType ItemType; // 아이템 타입

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Item")
	EWeaponType WeaponType; // 무기 타입

	FInventoryItem()
		: ItemID(TEXT("")), ItemName(TEXT("")), ItemDescription(TEXT("")), ItemClass(nullptr), ItemImage(nullptr), Quantity(0), ItemType(EItemType::It_None), WeaponType(EWeaponType::Wt_None)
	{}

	bool IsEmpty() const
	{
		return ItemID.IsEmpty() && ItemName.IsEmpty() && ItemDescription.IsEmpty() && ItemClass == nullptr && ItemImage == nullptr && Quantity == 0 && ItemType == EItemType::It_None && WeaponType == EWeaponType::Wt_None;
	}

	void Empty()
	{
		ItemID.Empty();
		ItemName.Empty();
		ItemDescription.Empty();
		ItemClass = nullptr;
		ItemImage = nullptr;
		Quantity = 0;
		ItemType = EItemType::It_None;
		WeaponType = EWeaponType::Wt_None;
	}

	void TestSword1()
	{
		ItemID = "KatanaID";
		ItemName = "Katana";
		ItemDescription = "This is Katana";
		ItemClass = nullptr;
		ItemImage = nullptr;
		Quantity = 1;
		ItemType = EItemType::It_Weapon;
		WeaponType = EWeaponType::Wt_Katana;
	}
	
	void TestSword2()
	{
		ItemID = "LongSwordID";
		ItemName = "LongSword";
		ItemDescription = "This is LongSword";
		ItemClass = nullptr;
		ItemImage = nullptr;
		Quantity = 1;
		ItemType = EItemType::It_Weapon;
		WeaponType = EWeaponType::Wt_LongSword;
	}
};
