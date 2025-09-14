// Fill out your copyright notice in the Description page of Project Settings.


// Sets default values

#include "ButtonActor.h"

#include "Lena/Characters/Base_Character.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Lena/Actor/InteractableThings/Door/DoorActor.h"
#include "Lena/Actor/Items/Inventory/InventoryComponent.h"
#include "Lena/Data/InventoryItem.h"
#include "Lena/UI/InteractWidget.h"

AButtonActor::AButtonActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	HitBox->SetRelativeLocation(FVector(0,70,0));
	HitBox->SetRelativeScale3D(FVector(3,2,4.25));
	WidgetComponent->SetRelativeLocation(FVector(0,0,70));

	ButtonMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ButtonMesh"));
	ButtonMesh->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AButtonActor::BeginPlay()
{
	Super::BeginPlay();
	
	if(ButtonMesh)
	{
		UMaterialInterface* Material = ButtonMesh->GetMaterial(0);
		UMaterialInterface* Material2 = ButtonMesh->GetMaterial(1);
		if (Material)
		{
			BaseMaterial = UMaterialInstanceDynamic::Create(Material, this);
			ButtonMesh->SetMaterial(0, BaseMaterial);

			BrightnessMaterial = UMaterialInstanceDynamic::Create(Material2, this);
			ButtonMesh->SetMaterial(1, BrightnessMaterial);
		}
	}
}

// Called every frame
void AButtonActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

bool AButtonActor::CheckConditions()
{
	ABase_Character* Character = Cast<ABase_Character>(UGameplayStatics::GetPlayerCharacter(this, 0));
	TMap<FString, bool> CheckItems;
	
	for(const FString Item : RequiredItem)
		CheckItems.Add(Item, false);

	for(const FInventoryItem& InventoryItem : Character->InventoryComponent->Items)
	{
		FString ItemName = InventoryItem.ItemName;
		if(CheckItems.Find(ItemName))
			CheckItems[ItemName] = true;
	}
	
	for(const TPair<FString, bool> ItemPair : CheckItems)
		if(ItemPair.Value == false)
			return false;

	return true;
}

void AButtonActor::OpenDoor()
{
	if(CheckConditions())
	{
		if(DoorActor)
		{
			DoorActor->RequiredCondition = "Default";
			SetInstructionWidgetText(FText::FromString("Opend!"), FLinearColor::Green);
			UGameplayStatics::PlaySoundAtLocation(GetWorld(), RightAnswerSound, GetActorLocation());
			DoorActor->Open();
			DestroyHitBoxAndWidgetDelayFunction(1.0);
			DoorActor->DestroyHitBoxAndWidgetDelayFunction(1.0);
			BrightnessMaterial->SetVectorParameterValue(TEXT("Emissive"), FLinearColor::Green);
		}
	}
	else
	{
		SetInstructionWidgetText(FText::FromString("Locked!"), FLinearColor::Red);
		ClearInstructionWidgetTextDelay(0.5);
		if(WrongAnswerCameraShakeClass)
			GetWorld()->GetFirstPlayerController()->ClientStartCameraShake(WrongAnswerCameraShakeClass);
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), WrongAnswerSound, GetActorLocation(),1.0f, 2.0f);
		BrightnessMaterial->SetVectorParameterValue(TEXT("Emissive"), FLinearColor::Red);
		GetWorld()->GetTimerManager().SetTimer(EmissiveHandle, this, &AButtonActor::OnEmissiveChange, 0.5f, false);
	}
}

void AButtonActor::OnEmissiveChange()
{
	BrightnessMaterial->SetVectorParameterValue(TEXT("Emissive"), FLinearColor::White);
}