// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterPlayerController.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "TimerManager.h"
#include "InputAction.h"
#include "InputMappingContext.h"
#include "Blueprint/UserWidget.h"
#include "Engine/BlueprintGeneratedClass.h"
#include "Lena/Characters/Base_Character.h"
#include "Lena/Actor/InteractableThings/Lock/ButtonLockActor.h"
#include "Kismet/GameplayStatics.h"
#include "Lena/GameInstanceSubSystem/LenaRoom01Manager.h"
#include "Lena/GameInstanceSubSystem/LenaUIManager.h"
#include "Lena/UI/InventoryWidget.h"
#include "Lena/UI/Lena_InGameMenuPannel.h"
#include "Lena/UI/Lena_InGameWIdget.h"

AShooterPlayerController::AShooterPlayerController()
{
	bFlipFlop = true;
}

void AShooterPlayerController::BeginPlay()
{
	Super::BeginPlay();
	
	// Character Reference caching
	if(ABase_Character* character =  Cast<ABase_Character>(UGameplayStatics::GetPlayerCharacter(GetWorld(),0)))
	{
		Base_Character = character;
	}
	// Item Info Widget AddViewport
	ItemInfoWidget->AddToViewport();
	ItemInfoWidget->SetInstructionAtBeginPlay(FText::FromString(""), FLinearColor::White);
	GetWorld()->GetTimerManager().SetTimer(TimerHandle_CheckPickUpItem, this, &AShooterPlayerController::CheckPickUpItemLine, 0.1f, true);
}

void AShooterPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	// 향상된 입력을 받는다. 이게 우리가 평소에 하던 입력에 들어가서 + 버튼을 누르고 액션을 추가해주던 것임
	if(UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(IMC_Interaction, 0);
	}
	if(UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent))
	{
		EnhancedInputComponent->BindAction(IA_MouseClick, ETriggerEvent::Triggered, this, &AShooterPlayerController::HandleMouseClick);
		EnhancedInputComponent->BindAction(IA_PickUpItem, ETriggerEvent::Triggered, this, &AShooterPlayerController::HandlePickUpItem);
		EnhancedInputComponent->BindAction(IA_OpenInventory, ETriggerEvent::Triggered, this, &AShooterPlayerController::HandleOpenInventory);
		EnhancedInputComponent->BindAction(IA_ToggleFlashLight, ETriggerEvent::Triggered, this, &AShooterPlayerController::OnToggleFlashLight);
		EnhancedInputComponent->BindAction(IA_OpenMenuInGame, ETriggerEvent::Triggered, this, &AShooterPlayerController::OnOpenMenuInGame);
	}
}

void AShooterPlayerController::OnToggleFlashLight(const FInputActionValue& value)
{
	const bool pressed = value.Get<bool>();
	if(pressed == true)
		Base_Character->ToggleFlashLight();
}

void AShooterPlayerController::OnOpenMenuInGame(const FInputActionValue& value)
{
	const bool pressed = value.Get<bool>();
	if(pressed == true)
	{
		if(bMenuShowed == false)
		{
			if(UBlueprintGeneratedClass* widgetclass = LoadObject<UBlueprintGeneratedClass>(nullptr, TEXT("/Game/UI/HUD/InGame/WBP_InGameMenuPannel.WBP_InGameMenuPannel_C")))
			{
				if(ULena_InGameMenuPannel* createdwidget = CreateWidget<ULena_InGameMenuPannel>(GetWorld(), widgetclass))
				{
					createdwidget->AddToViewport();
					MenuPannel = createdwidget;
				}
			}
			InputModeUI();
		}
		else
		{
			MenuPannel->RemoveFromParent();
			InputModeGame();
		}
		bMenuShowed = !bMenuShowed;
	}
}

void AShooterPlayerController::GameHasEnded(AActor* EndGameFocus, bool bIsWinner)
{
	Super::GameHasEnded(EndGameFocus, bIsWinner);
	
	if (bIsWinner)
	{
		// UUserWidget* WinScreen = CreateWidget(this, WinScreenClass);
		// if (WinScreen != nullptr)
		// 	WinScreen->AddToViewport();
		if(g_LenaUIManaerIsValid)
		{
			if(g_LenaUIManager->GetInGameWidget())
			{
				g_LenaUIManager->GetInGameWidget()->VisibleDescriptionTextRoom03Win();
				g_LenaUIManager->GetInGameWidget()->UpdateDescriptionTextRoom03Win(RestartDelay);
			}
		}
	}
	else
	{
		// UUserWidget* LoseScreen = CreateWidget(this, LoseScreenClass);
		// if (LoseScreen != nullptr)
		// 	LoseScreen->AddToViewport();
		if(g_LenaUIManaerIsValid)
		{
			if(g_LenaUIManager->GetInGameWidget())
			{
				g_LenaUIManager->GetInGameWidget()->VisibleDescriptionTextRoom03Win();
				g_LenaUIManager->GetInGameWidget()->UpdateDescriptionTextRoom03Win(RestartDelay);
			}
		}
	}
	GetWorldTimerManager().SetTimer(RestartTimer, this, &AShooterPlayerController::OpenIntroScene, RestartDelay);
	GetWorld()->GetTimerManager().SetTimer(CountdownTimerHandle, this, &AShooterPlayerController::UpdateCountdown, 1.0f, true);
}

void AShooterPlayerController::UpdateCountdown()
{
	if (RestartDelay > 0)
	{
		RestartDelay--; // 시간 감소
		if(g_LenaUIManaerIsValid)
		{
			if(g_LenaUIManager->GetInGameWidget())
			{
				g_LenaUIManager->GetInGameWidget()->UpdateDescriptionTextRoom03Win(RestartDelay);
			}
		}
	}
	else
	{
		// 타이머를 정지하고 카운트다운 타이머를 중지
		GetWorld()->GetTimerManager().ClearTimer(CountdownTimerHandle);
	}
}

void AShooterPlayerController::OpenIntroScene()
{
	UGameplayStatics::OpenLevel(GetWorld(), "IntroScene");
}

void AShooterPlayerController::EnableMouseClick()
{
	bIsClickEnabled = true;
}

void AShooterPlayerController::DisableMouseClick()
{
	bIsClickEnabled = false;
}

void AShooterPlayerController::HandleMouseClick()
{
	if(!bIsClickEnabled) return;
	
	float MouseX, MouseY;
	this->GetMousePosition(MouseX, MouseY);
	FVector2D ScreenPosition(MouseX, MouseY);
	FVector WorldLocation, WorldDirection;
	if (this->DeprojectScreenPositionToWorld(ScreenPosition.X, ScreenPosition.Y, WorldLocation, WorldDirection))
	{
		FVector Start = WorldLocation;
		FVector End = WorldLocation + (WorldDirection * 1000.0f); // Adjust the length of the ray as needed

		FHitResult HitResult;
		FCollisionQueryParams Params;
		Params.AddIgnoredActor(this);

		if (GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility, Params))
		{
			AActor* HitActor = HitResult.GetActor();
			if(HitActor)
			{

				ABase_Character* HitCharacter = Cast<ABase_Character>(HitActor);
				if (HitCharacter)
				{
					// Ignore the character and perform another line trace
					Params.AddIgnoredActor(HitActor);
					if (GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility, Params))
						HitActor = HitResult.GetActor();
					else
						HitActor = nullptr; // No valid actor hit
				}
				
				AButtonLockActor* ButtonLockActor = Cast<AButtonLockActor>(HitActor);
				if(ButtonLockActor)
				{
					UStaticMeshComponent* ButtonMeshComponent = Cast<UStaticMeshComponent>(HitResult.GetComponent());
					ButtonLockActor->MoveButton(ButtonMeshComponent);
				}
			}
		}
		// DrawDebugLine(GetWorld(), Start, End, FColor::Green, false, 1.0f, 0, 1.0f);
	}
}

///// Player Widget
void AShooterPlayerController::InputModeUI()
{
	bShowMouseCursor = true;
	SetIgnoreLookInput(true);
	SetIgnoreMoveInput(true);
	FInputModeGameAndUI InputMode;
	InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	SetInputMode(InputMode);
	IsInWidget = true;
}

///// Player Widget
void AShooterPlayerController::InputModeUIOnly()
{
	bShowMouseCursor = true;
	SetIgnoreLookInput(true);
	SetIgnoreMoveInput(true);
	SetInputMode(FInputModeUIOnly());
	IsInWidget = true;
}

void AShooterPlayerController::InputModeGame()
{
	SetInputMode(FInputModeGameOnly());
	bShowMouseCursor = false;
	SetIgnoreLookInput(false);
	SetIgnoreMoveInput(false);
	IsInWidget = false;
}

void AShooterPlayerController::HandlePickUpItem()
{
	if(LineTraceItem && Base_Character)
	{
		Base_Character->PickupItem(LineTraceItem, EInventorySlotType::ISt_Ground);
	}
}

void AShooterPlayerController::CheckPickUpItemLine()
{
	FVector Start = PlayerCameraManager->GetCameraLocation() + (PlayerCameraManager->GetActorForwardVector() * 30.0f);;
	FVector End = PlayerCameraManager->GetCameraLocation() + (PlayerCameraManager->GetActorForwardVector() * 400.0f); // Adjust the length of the ray as needed

	FHitResult HitResult;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);
	float Radius = 15.0f;

	if (GetWorld()->SweepSingleByChannel(HitResult, Start, End, FQuat::Identity, ECC_Visibility,FCollisionShape::MakeSphere(Radius), Params))
	{
		AActor* HitActor = HitResult.GetActor();
		if(HitActor)
		{
			ABase_Item* Item = Cast<ABase_Item>(HitActor);
			if(Item)
			{
				ItemInfoWidget->SetInstruction(FText::FromString(Item->ItemName));
				LineTraceItem = Item;
				return;
			}
		}
	}
	
	ItemInfoWidget->SetInstruction(FText::FromString(""));
	LineTraceItem = nullptr;
	// DrawDebugLine(GetWorld(), Start, End, FColor::Green, false, 1.0f, 0, 1.0f);
	// DrawDebugSphere(GetWorld(), End, Radius, 3, FColor::Red, false, 1.0f, 0, 1.0f);
}

void AShooterPlayerController::CheckPickUpItemSweep()
{
	ACharacter* Character_ =  UGameplayStatics::GetPlayerCharacter(GetWorld(),0);
	FVector Start = Character_->GetActorLocation();
	FVector End = Character_->GetActorLocation();

	TArray<FHitResult> HitResults;
	FCollisionQueryParams Params;
	float Radius = 130.0f;
	if(GetWorld()->SweepMultiByChannel(HitResults, Start, End, FQuat::Identity, ECC_Visibility, FCollisionShape::MakeSphere(Radius), Params))
	{
		TSet<AActor*> ProcessedActors;
		if(Base_Character->GroundItemsComponent.IsValid())
		{
			Base_Character->GroundItemsComponent.Get()->Items.Empty();
		
			for(FHitResult HitResult : HitResults )
			{
				AActor* HitActor = HitResult.GetActor();
				if(HitActor && !ProcessedActors.Contains(HitActor))
				{
					ProcessedActors.Add(HitActor);
					ABase_Item* Item = Cast<ABase_Item>(HitActor);
					if(Item)
						if(Base_Character)
							Base_Character->CheckGroundItem(Item);
				}
			}
		}
	}
}

// Tab To Open Inventory
void AShooterPlayerController::HandleOpenInventory(const FInputActionValue& ActionValue)
{
	bool bIsPressed = ActionValue.Get<bool>();
	CheckPickUpItemSweep();
	
	if(bIsPressed)
	{
		if(bFlipFlop)
		{
			if(InventoryWidget)
			{
				if(InventoryWidget->IsInViewport())
				{
					InventoryWidget->SetVisibility(ESlateVisibility::Visible);
					InputModeUI();
					InventoryWidget->BuildInventorySlot();
					InventoryWidget->BuildGroundSlot();
					InventoryWidget->BuildEquipmentSlot();
				}
				else
				{
					InventoryWidget->AddToViewport();
					InventoryWidget->SetPlayerController(this);
					if(Base_Character)
						InventoryWidget->SetBaseCharacter(Base_Character);
					InputModeUI();
					InventoryWidget->BuildInventorySlot();
					InventoryWidget->BuildGroundSlot();
					InventoryWidget->BuildEquipmentSlot();
				}
			}
		}
		else
		{
			InventoryWidget->SetVisibility(ESlateVisibility::Collapsed);
			InputModeGame();
		}

		bFlipFlop = !bFlipFlop;
	}
}
