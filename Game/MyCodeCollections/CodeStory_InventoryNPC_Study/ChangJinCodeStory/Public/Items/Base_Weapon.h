// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Base_Item.h"
#include "Data/InventoryItem.h"
#include "Base_Weapon.generated.h"

UCLASS()
class CHANGJINCODESTORY_API ABase_Weapon : public ABase_Item
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ABase_Weapon();

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

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
