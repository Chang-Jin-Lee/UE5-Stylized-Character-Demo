// Copyright Epic Games, Inc. All Rights Reserved.

#include "ChangJinCodeStory/Public/Characters/Base_Character.h"

#include "Blueprint/UserWidget.h"
#include "Components/InputComponent.h"
#include "GameFramework/Controller.h"
#include "Kismet/GameplayStatics.h"
#include "Widgets/InventoryUIWidget.h"
#include "Components/InventoryComponent.h"

//////////////////////////////////////////////////////////////////////////
// ABase_Character

ABase_Character::ABase_Character()
{
	InventoryComponent = CreateDefaultSubobject<UInventoryComponent>(TEXT("InventoryComponent"));
}

void ABase_Character::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();
}

void ABase_Character::OpenInventory()
{
	if (PlayerUIWidget)
	{
		if (PlayerUIWidget->IsInViewport())
		{
			CloseInventory();
		}
		else
		{
			ShowInventory();
		}
	}
	else
	{
		CreateAndShowInventory();
	}

}

void ABase_Character::CloseInventory()
{
	PlayerUIWidget->RemoveFromParent();
	PlayerUIWidget = nullptr;
	InputModeGame();
}

void ABase_Character::ShowInventory()
{
	if (PlayerUIWidget)
	{
		PlayerUIWidget->AddToViewport();
		PlayerUIWidget->SetVisibility(ESlateVisibility::Visible);
		InputModeUI();
	}
}

void ABase_Character::CreateAndShowInventory()
{
	PlayerUIWidget = CreateWidget<UInventoryUIWidget>(GetWorld(), UInventoryUIWidget::StaticClass());
	if (PlayerUIWidget)
	{
		PlayerUIWidget->Rows = InventoryGridCount.X;
		PlayerUIWidget->Columns = InventoryGridCount.Y;
		PlayerUIWidget->Base_Character = this;

		PlayerUIWidget->InitializeInventory();

		PlayerUIWidget->SetPositionInViewport(InventoryWidgetPosition);
		ShowInventory();
	}
}

void ABase_Character::InputModeUI()
{
	APlayerController* pc = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	pc->bShowMouseCursor = true;
	pc->SetIgnoreLookInput(true);
	pc->SetInputMode(FInputModeGameAndUI());
}


void ABase_Character::InputModeGame()
{
	APlayerController* pc = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	pc->bShowMouseCursor = false;
	pc->SetIgnoreLookInput(false);
	pc->SetIgnoreMoveInput(false);
	pc->SetInputMode(FInputModeGameOnly());
}

void ABase_Character::InventoryReload()
{
	if(PlayerUIWidget)
	{
		PlayerUIWidget->InitializeInventory();
	}
}
