// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemActor.h"

#include "InventoryData.h"
#include "InventoryDataTableManager.h"
#include "Camera/CameraComponent.h"
#include "CJSlate/CJSlateCharacter.h"
#include "CJSlate/UI/ItemWidget.h"
#include "Components/BoxComponent.h"
#include "Components/WidgetComponent.h"
#include "Net/UnrealNetwork.h"

AItemActor::AItemActor()
{
	PrimaryActorTick.bCanEverTick = true;

	bReplicates = true;
	SetReplicateMovement(true);

	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	WidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("WidgetComponent"));
	RadiusBoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));

	SetRootComponent(SceneComponent);
	StaticMeshComponent->SetupAttachment(SceneComponent);
	WidgetComponent->SetupAttachment(SceneComponent);
	RadiusBoxComponent->SetupAttachment(SceneComponent);
	
	m_cCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	m_cCameraComponent->SetupAttachment(SceneComponent);
}

void AItemActor::LoadData()
{
	if (ItemDataTableManagerptr == nullptr)
		ItemDataTableManagerptr = NewObject<UInventoryDataTableManager>(this, UInventoryDataTableManager::StaticClass());
	FInventoryItemData Data = ItemDataTableManagerptr->GetMatchingItemByTagAndName(m_fitemInfo.ItemTag, m_fitemInfo.ItemName);
	if (Data.ItemID != -1)
	{
		m_fitemInfo.ItemName = Data.ItemName;
		m_fitemInfo.ItemTag = Data.ItemTags;
		m_fitemInfo.ItemID = Data.ItemID;
		m_fitemInfo.ItemDescription = Data.ItemDescription;
		m_fitemInfo.ItemImage = Data.ItemImage;
		if (m_fitemInfo.ItemMesh)
		{
			StaticMeshComponent->SetStaticMesh(m_fitemInfo.ItemMesh);
		}
	}
}

void AItemActor::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AItemActor, m_fitemInfo);
}

void AItemActor::BeginPlay()
{
	Super::BeginPlay();

	WidgetComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	UUserWidget* widget = WidgetComponent->GetWidget();
	if (widget)
		widget->SetVisibility(ESlateVisibility::Hidden);

	LoadData();
	
	const FGameplayTag PortalTag = FGameplayTag::RequestGameplayTag(FName("Item.World.Portal"));
	if (m_fitemInfo.ItemTag.HasTag(PortalTag) && !IsAnswerPortal)
	{
		FString CurrentLevelName = GetWorld()->GetMapName();

		if (CurrentLevelName.Contains("Wrong"))
		{
			TArray<FString> WrongLevels = {
				TEXT("Room01_Wrong01"),
				TEXT("Room01_Wrong02"),
				TEXT("Room01_Wrong03"),
				TEXT("Room01_Wrong04")
			};
			int32 RandomIndex = FMath::RandRange(0, WrongLevels.Num() - 1);
			m_fitemInfo.PortalLevel = WrongLevels[RandomIndex];
		}
	}
	
}

void AItemActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AItemActor::DestroyItem_Implementation()
{
	Destroy(true);
}

bool AItemActor::DestroyItem_Validate()
{
	return true;
}

void AItemActor::OnRep_ItemInfo_Implementation()
{
	LoadData();
}

void AItemActor::ShowWidget()
{
	if (WidgetComponent)
	{
		UItemWidget* widget = Cast<UItemWidget>(WidgetComponent->GetWidget());
		if (widget)
		{
			widget->SetTextBlock(m_fitemInfo.ItemName);
			widget->SetVisibility(ESlateVisibility::Visible);
		}
	}
}

void AItemActor::HideWidget()
{
	if (WidgetComponent)
	{
		UItemWidget* widget = Cast<UItemWidget>(WidgetComponent->GetWidget());
		if (widget)
		{
			widget->SetTextBlock(m_fitemInfo.ItemName);
			widget->SetVisibility(ESlateVisibility::Hidden);
		}
	}
}

