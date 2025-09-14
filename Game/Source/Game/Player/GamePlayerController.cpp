#include "Game/Player/GamePlayerController.h"
#include "Game/CharacterSystem/CharacterSelectionSubsystem.h"
#include "Engine/World.h"
#include "Engine/GameViewportClient.h"
#include "Game/GameCharacter.h"
#include "Game/UI/PlayerOverlayWidget.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "InputCoreTypes.h"
#include "Game/UI/InventoryWidget.h"
#include "Game/Input/CharacterInputConfig.h"
#include "Game/CharacterSystem/CharacterSystemSettings.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/CanvasPanelSlot.h"

/*
 * @brief : ���� �� ����� ĳ���ͷ� ����/����
 * @details : GameInstanceSubsystem�� ���� ���ð��� �����ϰ� Pawn ��ü
 */
void AGamePlayerController::BeginPlay()
{
	Super::BeginPlay();
	if (UCharacterSelectionSubsystem* Subsystem = GetCharacterSubsystem())
	{
		// ù ���� �� ����� ĳ���ͷ� ����/����
		Subsystem->SpawnOrSwapPlayer(GetWorld(), this);
	}

	// Overlay�� Pawn�� ����� �� ����
	if (GetPawn() && GEngine && GEngine->GameViewport)
	{
		SAssignNew(PlayerOverlay, SPlayerOverlayWidget).Player(Cast<AGameCharacter>(GetPawn()));
		GEngine->GameViewport->AddViewportWidgetContent(PlayerOverlay.ToSharedRef(), 10);
	}

	// Bind once
	if (!bUIActionsBound)
	{
		if (UEnhancedInputComponent* EIC = Cast<UEnhancedInputComponent>(InputComponent))
		{
			// Ensure actions
			if (!InventoryAction) { InventoryAction = NewObject<UInputAction>(this, TEXT("IA_Inventory_Temp")); }
			if (!InteractAction)  { InteractAction  = NewObject<UInputAction>(this, TEXT("IA_Interact_Temp")); }
			if (!CtrlAction)      { CtrlAction      = NewObject<UInputAction>(this, TEXT("IA_Ctrl_Temp")); }

			// Map fallback keys into a runtime IMC
			if (ULocalPlayer* LP = GetLocalPlayer())
			{
				if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(LP))
				{
					UInputMappingContext* RuntimeIMC = NewObject<UInputMappingContext>(this, TEXT("RuntimeInput_PC"));
					RuntimeIMC->MapKey(InventoryAction, EKeys::I);
					RuntimeIMC->MapKey(InteractAction, EKeys::F);
					RuntimeIMC->MapKey(CtrlAction, EKeys::LeftControl);
					Subsystem->AddMappingContext(RuntimeIMC, 255);
				}
			}

			// Bind actions
			EIC->BindAction(InteractAction, ETriggerEvent::Started, this, &AGamePlayerController::OnInteractStarted);
			EIC->BindAction(InventoryAction, ETriggerEvent::Started, this, &AGamePlayerController::OnInventoryStarted);
			EIC->BindAction(CtrlAction, ETriggerEvent::Started, this, &AGamePlayerController::OnCtrlPressed);
			EIC->BindAction(CtrlAction, ETriggerEvent::Completed, this, &AGamePlayerController::OnCtrlReleased);

			bUIActionsBound = true;
		}
	}
}

void AGamePlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	// ��ȭ�� �Է� ���� ���ؽ�Ʈ ���� ����
	if (IsLocalController())
	{
		if (ULocalPlayer* LP = GetLocalPlayer())
		{
			if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(LP))
			{
				if (const UCharacterSystemSettings* Settings = GetDefault<UCharacterSystemSettings>())
				{
					const UCharacterInputConfig* InputCfg = Settings->DefaultInputConfig.IsValid() ? Settings->DefaultInputConfig.LoadSynchronous() : nullptr;
					if (InputCfg)
					{
						if (InputCfg->MappingContext.IsValid())
						{
							Subsystem->AddMappingContext(InputCfg->MappingContext.LoadSynchronous(), InputCfg->MappingPriority);
						}
						// Ensure I/F keys exist in mapping (fallback)
						UInputMappingContext* RuntimeIMC = NewObject<UInputMappingContext>(this, TEXT("RuntimeInputConfig_Controller"));
						if (InventoryAction)
						{
							RuntimeIMC->MapKey(InventoryAction, EKeys::I);
						}
						if (InteractAction)
						{
							RuntimeIMC->MapKey(InteractAction, EKeys::F);
						}
						Subsystem->AddMappingContext(RuntimeIMC, InputCfg->MappingPriority + 2);
					}
				}
			}
		}
	}

	if (GEngine && GEngine->GameViewport)
	{
		if (!PlayerOverlay.IsValid())
		{
			SAssignNew(PlayerOverlay, SPlayerOverlayWidget).Player(Cast<AGameCharacter>(InPawn));
			GEngine->GameViewport->AddViewportWidgetContent(PlayerOverlay.ToSharedRef(), 10);
		}
	}
}

void AGamePlayerController::OnCtrlPressed()
{
	bShowMouseCursor = true;
	FInputModeGameAndUI Mode;
	Mode.SetHideCursorDuringCapture(false);
	Mode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	SetInputMode(Mode);
}

void AGamePlayerController::OnCtrlReleased()
{
	bShowMouseCursor = false;
	FInputModeGameOnly Mode;
	SetInputMode(Mode);
}

/*
 * @brief : ĳ���� ����ý��� ������
 * @details : GameInstance�κ��� Subsystem ȹ��
 */
UCharacterSelectionSubsystem* AGamePlayerController::GetCharacterSubsystem() const
{
	if (UGameInstance* GI = GetGameInstance())
	{
		return GI->GetSubsystem<UCharacterSelectionSubsystem>();
	}
	return nullptr;
}

/*
 * @brief : PrimaryAssetId ���ڿ��� ĳ���� ���� ��û
 * @details : ���� ���� �� ��� ����/����
 */
void AGamePlayerController::RequestSelectCharacter(const FString& PrimaryAssetIdString)
{
	if (UCharacterSelectionSubsystem* Subsystem = GetCharacterSubsystem())
	{
		Subsystem->SelectCharacterById(PrimaryAssetIdString);
		Subsystem->SpawnOrSwapPlayer(GetWorld(), this);
	}
}

void AGamePlayerController::CharList()
{
	if (UCharacterSelectionSubsystem* Subsystem = GetCharacterSubsystem())
{
		Subsystem->ScanCharacterAssets();
		TArray<FString> Ids;
		Subsystem->GetAllCharacterIds(Ids);
		for (const FString& Id : Ids)
		{
			ClientMessage(FString::Printf(TEXT("Character: %s"), *Id));
			UE_LOG(LogTemp, Log, TEXT("Character: %s"), *Id);
		}
	}
}

void AGamePlayerController::CharSelect(const FString& PrimaryAssetIdString)
{
	RequestSelectCharacter(PrimaryAssetIdString);
}

void AGamePlayerController::OnInteractStarted(const FInputActionInstance& Instance)
{
	UE_LOG(LogTemp, Verbose, TEXT("Interact pressed"));
	APawn* P = GetPawn();
	if (!P) return;
	FVector Start = P->GetActorLocation();
	FVector End = Start + P->GetActorForwardVector() * 250.f;
	FHitResult Hit;
	FCollisionQueryParams Params; Params.AddIgnoredActor(P);
	if (GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Pawn, Params))
	{
		if (AActor* HitActor = Hit.GetActor())
		{
			CurrentShopTarget = HitActor;
			// UMG �켱
			if (ShopWidgetClass)
			{
				if (ShopWidget.IsValid())
				{
					ShopWidget->RemoveFromParent();
					ShopWidget.Reset();
				}
				else
				{
					ShopWidget = CreateWidget<UUserWidget>(this, ShopWidgetClass);
					if (ShopWidget.IsValid())
					{
						ShopWidget->AddToViewport(20);
						const FVector2D ViewSize = UWidgetLayoutLibrary::GetViewportSize(this);
						if (UCanvasPanelSlot* Slot = UWidgetLayoutLibrary::SlotAsCanvasSlot(ShopWidget.Get()))
						{
							Slot->SetPosition(FVector2D(48.f, ViewSize.Y * 0.2f));
						}
					}
				}
			}
			// Slate ����(�׸��� ��Ÿ�� ���� ����)
			else
			{
				if (ShopOverlayRoot.IsValid())
				{
					GEngine->GameViewport->RemoveViewportWidgetContent(ShopOverlayRoot.ToSharedRef());
					ShopOverlayRoot.Reset();
				}
				else if (GEngine && GEngine->GameViewport)
				{
					TSharedRef<SOverlay> Overlay = SNew(SOverlay)
						+ SOverlay::Slot().HAlign(HAlign_Left).VAlign(VAlign_Top).Padding(FMargin(48.f, 96.f))
						[
							SNew(SInventoryWidget)
							.Rows(4)
							.Cols(6)
							.SlotSize(FVector2D(72.f, 72.f))
							.SlotPadding(FMargin(4.f))
						];
					ShopOverlayRoot = Overlay;
					GEngine->GameViewport->AddViewportWidgetContent(Overlay, 20);
				}
			}

			// ���� �� �� UI ��Ŀ���� Ŀ�� ����
			bShowMouseCursor = true;
			FInputModeGameAndUI Mode; Mode.SetHideCursorDuringCapture(false); Mode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
			SetInputMode(Mode);
		}
	}
}

void AGamePlayerController::OnInventoryStarted(const FInputActionInstance& Instance)
{
	UE_LOG(LogTemp, Verbose, TEXT("Inventory pressed"));
	// UMG ���
	if (InventoryWidgetClass)
	{
		if (InventoryWidget.IsValid())
		{
			InventoryWidget->RemoveFromParent();
			InventoryWidget.Reset();
			bShowMouseCursor = false; SetInputMode(FInputModeGameOnly());
		}
		else
		{
			InventoryWidget = CreateWidget<UUserWidget>(this, InventoryWidgetClass);
			if (InventoryWidget.IsValid())
			{
				InventoryWidget->AddToViewport(20);
				const FVector2D ViewSize = UWidgetLayoutLibrary::GetViewportSize(this);
				if (UCanvasPanelSlot* Slot = UWidgetLayoutLibrary::SlotAsCanvasSlot(InventoryWidget.Get()))
				{
					// �����ʿ� ������ ����(48px)�� ��� ����(96px)
					Slot->SetPosition(FVector2D(ViewSize.X - 48.f - 72.f * 6.f, ViewSize.Y * 0.2f));
				}
				bShowMouseCursor = true; FInputModeGameAndUI Mode; Mode.SetHideCursorDuringCapture(false); Mode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock); SetInputMode(Mode);
			}
		}
		return;
	}

	// Slate ����
	if (InventoryOverlayRoot.IsValid())
	{
		GEngine->GameViewport->RemoveViewportWidgetContent(InventoryOverlayRoot.ToSharedRef());
		InventoryOverlayRoot.Reset();
		InventoryUI.Reset();
		bShowMouseCursor = false; SetInputMode(FInputModeGameOnly());
	}
	else
	{
		TSharedRef<SOverlay> Overlay = SNew(SOverlay)
			+ SOverlay::Slot().HAlign(HAlign_Right).VAlign(VAlign_Top).Padding(FMargin(48.f, 96.f))
			[
				SAssignNew(InventoryUI, SInventoryWidget)
				.Rows(4)
				.Cols(6)
				.SlotSize(FVector2D(72.f, 72.f))
				.SlotPadding(FMargin(4.f))
			];
		InventoryOverlayRoot = Overlay;
		GEngine->GameViewport->AddViewportWidgetContent(Overlay, 20);
		bShowMouseCursor = true; FInputModeGameAndUI Mode; Mode.SetHideCursorDuringCapture(false); Mode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock); SetInputMode(Mode);
	}
}

void AGamePlayerController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	// NPC �þ� ���� ����� ���� �ݱ�(���� �Ÿ� üũ)
	if (CurrentShopTarget.IsValid())
	{
		APawn* P = GetPawn(); if (!P) return;
		const float Dist = FVector::Dist(CurrentShopTarget->GetActorLocation(), P->GetActorLocation());
		const float MaxRange = 300.f + ShopClosePadding;
		if (Dist > MaxRange)
		{
			if (ShopWidget.IsValid()) { ShopWidget->RemoveFromParent(); ShopWidget.Reset(); }
			if (ShopOverlayRoot.IsValid() && GEngine) { GEngine->GameViewport->RemoveViewportWidgetContent(ShopOverlayRoot.ToSharedRef()); ShopOverlayRoot.Reset(); }
			CurrentShopTarget.Reset();
		}
	}
}


