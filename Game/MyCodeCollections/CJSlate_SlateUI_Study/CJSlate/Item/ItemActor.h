// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "GameFramework/Actor.h"
#include "ItemActor.generated.h"

class UCameraComponent;
class UInventoryDataTableManager;
class UWidgetComponent;
class UBoxComponent;

// 인벤토리에 아이템을 넣기 위해 만든 구조체.
// ItemActor에 변수를 추가하면 여기에도 추가해야함.
USTRUCT(BlueprintType)
struct FItemActorInfo
{
	GENERATED_BODY();
	
	// 이 아이템이 몇 번 아이템인지 확인하기 위함
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Item")
	int32 ItemID = -1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Item")
	FString ItemName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Item")
	UTexture2D* ItemImage;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Item")
	FText ItemDescription;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Item")
	int32 ItemCount = 0;

	// 포탈 아이템에서 어떤 레벨로 이동할지
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Item")
	FString PortalLevel;

	// 포탈 아이템에서 어떤 위치로 이동할지
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Item")
	FString PortalLocation;

	// 이 아이템이 무슨 아이템인지 확인하기 위함
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Item")
	FGameplayTagContainer ItemTag;

	// 아이템 메쉬 추가
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	UStaticMesh* ItemMesh; 
};

UCLASS()
class CJSLATE_API AItemActor : public AActor
{
	GENERATED_BODY()

public:
	AItemActor();

	void LoadData();

	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	
protected:
	virtual void BeginPlay() override;
	
public:
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Item")
	UInventoryDataTableManager* ItemDataTableManagerptr;

	UPROPERTY(EditDefaultsOnly)
	USceneComponent* SceneComponent;

	UPROPERTY(EditDefaultsOnly)
	UStaticMeshComponent* StaticMeshComponent;

	UPROPERTY(EditDefaultsOnly)
	UWidgetComponent* WidgetComponent;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	UBoxComponent* RadiusBoxComponent;

	UPROPERTY(EditAnywhere, Category="Camera")
	UCameraComponent* m_cCameraComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, ReplicatedUsing="OnRep_ItemInfo", Category = "Item")
	FItemActorInfo m_fitemInfo;

	UFUNCTION(Client, Reliable)
	void OnRep_ItemInfo();

	UFUNCTION(NetMulticast, Reliable, WithValidation)
	void DestroyItem();
	
	void ShowWidget();
	void HideWidget();

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Portal")
	bool IsAnswerPortal = true;
};