// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputMappingContext.h"
#include "Lena/Actor/Items/Base_Item.h"
#include "Lena/Actor/Items/Base_Weapon.h"
#include "Lena/Actor/Items/Gun.h"
#include "GameFramework/Character.h"
#include "Lena/Actor/Items/Inventory/InventoryComponent.h"
#include "Lena/Data/CharacterCommon.h"
#include "Base_Character.generated.h"

class APlayerCameraManager;
class USpotLightComponent;

UCLASS()
class LENA_API ABase_Character : public ACharacter
{
	GENERATED_BODY()

public:
	ABase_Character();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	USkeletalMeshComponent* FindMeshByName(FName MeshName);

	FORCEINLINE void SetIsAimed(bool _in) {IsAimed = _in;}
	FORCEINLINE float GetHealth() {return Health;}
	FORCEINLINE ECharacterStateType GetCharacterState() {return CharacterState;}
	// About Gun
	UFUNCTION()
	void ToggleIsArmed();
	// void Shoot();
	UFUNCTION()
	void PerformAction();
	void ReloadAction();
	void ReloadAIAction();
	void ChangeWeapon1();
	void ChangeWeapon2();
	void ToggleFlashLight();
	// void HideAllWeapons();
	
	UFUNCTION(BlueprintCallable)
	void ChangeWeapon(int32 Index);
	
	UFUNCTION(BlueprintCallable)
	void DrawWeapon(int32 WeaponIndex);

	UFUNCTION(BlueprintCallable)
	void PutDownWeapon();
	
	UFUNCTION(BlueprintCallable)
	ABase_Weapon* GetWeapon();

	UFUNCTION(BlueprintCallable)
	bool CheckAmmo();
	
	UFUNCTION(BlueprintCallable)
	int32 GetAmmoIndex(EAmmoType At);
	
	// UFUNCTION(BlueprintCallable)
	// void EquipItem(const FInventoryItem& InventoryItem, int32 Index);
	
	// UFUNCTION(BlueprintCallable)
	// void ChangeWeapon();
	//
	// UFUNCTION(BlueprintCallable)
	// AGun* GetWeapon();
	//
	// UFUNCTION(BlueprintCallable)
	// int GetAmmo();
	//
	// UFUNCTION(BlueprintCallable)
	// void AddAmmo_5mm();
	
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	UFUNCTION(BlueprintPure)
	bool IsDead(); // 이 함수를 호출했다고 캐릭터의 어떠한 상태도 바뀌면 안되기 때문. 안전하게 const로 선언

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool IsAimed = false;
	
	UFUNCTION(BlueprintCallable)
	bool ReloadGun();

	UFUNCTION(BlueprintImplementableEvent, Category = "Reload")
	void ReloadEvent();

	UFUNCTION(BlueprintPure)
	float GetHealthPercent() const;

	UPROPERTY()
	FString OverLayString;

	UFUNCTION(BlueprintCallable, Category = "OverLay Slot")
	void SetOverLayString(FString st);

	UFUNCTION(BlueprintCallable, Category = "OverLay Slot")
	void AdjustOverLay();

	UFUNCTION(BlueprintCallable)
	bool GetIsAICharacter();

	UFUNCTION(BlueprintImplementableEvent, Category = "OverLay Slot")
	void DrawWeaponEvent();

	UFUNCTION(BlueprintImplementableEvent, Category = "OverLay Slot")
	void PutDownWeaponEvent();

	UFUNCTION(BlueprintImplementableEvent, Category = "Animation Slot")
	void OnDead();

	// -----------------------------
	// Inventory
	// -----------------------------

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weapons")
	TObjectPtr<ABase_Weapon> CurrentWeapon;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weapons")
	UInventoryComponent* WeaponSlotsComponentptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Inventory")
	UInventoryComponent* InventoryComponentptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Inventory")
	UInventoryComponent* GroundItemsComponentptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weapons")
	TWeakObjectPtr<UInventoryComponent> WeaponSlotsComponent;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Inventory")
	TWeakObjectPtr<UInventoryComponent> InventoryComponent;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Inventory")
	TWeakObjectPtr<UInventoryComponent> GroundItemsComponent;
	
	UFUNCTION(BlueprintCallable, Category="Inventory")
	void PickupItem(AActor* ItemActor, EInventorySlotType InventorySlot);
	
	UFUNCTION(BlueprintCallable, Category="Inventory")
	void DropItem(FInventoryItem ItemData, EInventorySlotType InventorySlot);

	UFUNCTION(BlueprintCallable, Category="Inventory")
	void EquipItem(FInventoryItem ItemData, EInventorySlotType InventorySlot, int32 SlotIndex);
	
	UFUNCTION(BlueprintCallable, Category="Inventory")
	void CheckGroundItem(AActor* ItemActor);

	UPROPERTY(EditAnywhere)
	TObjectPtr<USoundBase> DropItemSound;
	
	UPROPERTY(EditAnywhere)
	TObjectPtr<USoundBase> PickupItemSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<USpotLightComponent> SpotLightComponent;

	UPROPERTY(BlueprintReadWrite, Category="Camera")
	TObjectPtr<APlayerCameraManager> CameraManager;

private:
	UPROPERTY(EditDefaultsOnly)
	float MaxHealth = 100;
	
	UPROPERTY(VisibleAnywhere)
	float Health;

	UPROPERTY()
	bool IsAICharacter = false;
	
	UPROPERTY(BlueprintReadWrite, Category="Weapon", meta=(AllowPrivateAccess="true"))
	bool IsArmed = false;

	UPROPERTY(BlueprintReadWrite, Category="Ammo", meta=(AllowPrivateAccess="true"))
	int Ammo_5mm = 60;
	ECharacterStateType CharacterState;
};