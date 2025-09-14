// Copyright Epic Games, Inc. All Rights Reserved.

#include "CJSlateCharacter.h"

#include "CJSlatePlayerController.h"
#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/BoxComponent.h"
#include "GameFramework/GameMode.h"
#include "Item/ItemActor.h"
#include "Kismet/GameplayStatics.h"
#include "Materials/MaterialInstanceConstant.h"
#include "UI/SCJSlateSpeechBubble.h"
#include "Widgets/SWeakWidget.h"

struct FSChatMsgInfo;
DEFINE_LOG_CATEGORY(LogTemplateCharacter);
DECLARE_DELEGATE_OneParam(MyDelegate, int);

//////////////////////////////////////////////////////////////////////////
// ACJSlateCharacter

ACJSlateCharacter::ACJSlateCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
		
	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;
	
	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)
}

void ACJSlateCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (GEngine && GEngine->GameViewport)
	{
		SpeechBubblePtr = SNew(SCJSlateSpeechBubble)
			.BackgroundColor(FLinearColor::Transparent)
			.Message(FText::FromString(TEXT("SpeechBubble!")));
		
		GEngine->GameViewport->AddViewportWidgetContent(SNew(SWeakWidget).PossiblyNullContent(SpeechBubblePtr.ToSharedRef()));
		if (SpeechBubblePtr.IsValid())
		{
			SpeechBubblePtr->SetMessage(FText::FromString(TEXT("SpeechBubble!")));
		}
	}

	
	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &ACJSlateCharacter::OnCharacterComponentBeginOverlap);
	GetCapsuleComponent()->OnComponentEndOverlap.AddDynamic(this, &ACJSlateCharacter::OnCharacterComponentEndOverlap);
}

void ACJSlateCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	UpdateSpeechBubblePosition();
}

void ACJSlateCharacter::UpdateSpeechBubblePosition()
{
	if (!SpeechBubblePtr.IsValid() || !GetWorld()) return;

	// world position을 screen position 로 변경
	APlayerController* PlayerController = Cast<APlayerController>(GetWorld()->GetFirstPlayerController());
	if (!PlayerController || !PlayerController->IsLocalController()) return; // 로컬 컨트롤러에서만 실행

	FVector WorldPosition = GetActorLocation() + FVector(0, 0, 120); // 캐릭터의 머리 위에 말풍선을 위치시킴
	FVector2D ScreenPosition;
	
	if (UGameplayStatics::ProjectWorldToScreen(PlayerController, WorldPosition, ScreenPosition))
	{
		if (SpeechBubblePtr)
		{
			float DPI = UWidgetLayoutLibrary::GetViewportScale(PlayerController);
			FVector2D AdjustedScreenPosition = ScreenPosition / DPI;	// ScreenPosition의 DPI 사이즈 계산
			FVector2D BubbleSize = SpeechBubblePtr->GetDesiredSize(); // 말풍선의 사이즈
			FVector2D CenteredPosition = AdjustedScreenPosition - (BubbleSize / 2.0f); // 찾은 위치를 말풍선의 중심으로 두기
			SpeechBubblePtr->SetRenderTransform(FSlateRenderTransform(CenteredPosition)); // 말풍선 그리기
		}
	}
}

void ACJSlateCharacter::SetMessage(const FSChatMsgInfo& newmessage)
{
	if (SpeechBubblePtr.IsValid())
		SpeechBubblePtr->SetMessage(newmessage.m_tText);
}

void ACJSlateCharacter::Multicast_UpdateMessage_Implementation(const FSChatMsgInfo& newmessage)
{
	SetMessage(newmessage);
}

//////////////////////////////////////////////////////////////////////////
// Input

void ACJSlateCharacter::NotifyControllerChanged()
{
	Super::NotifyControllerChanged();

	// Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
}

void ACJSlateCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ACJSlateCharacter::Move);
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ACJSlateCharacter::Look);
		EnhancedInputComponent->BindAction(MainMenuAction, ETriggerEvent::Triggered, this, &ACJSlateCharacter::ShowMainMenu);
		EnhancedInputComponent->BindAction(OpenInventoryAction, ETriggerEvent::Triggered, this, &ACJSlateCharacter::OpenInventory);
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void ACJSlateCharacter::ChangeMaterialColor_Implementation(FLinearColor Color)
{
	if (USkeletalMeshComponent* skeletalmesh = GetMesh())
	{
		TArray<UMaterialInterface*> MaterialInterfaces = skeletalmesh->GetMaterials();
		for (int32 i = 0 ; i <  MaterialInterfaces.Num(); i++)
		{
			UMaterialInterface* MaterialInterface = MaterialInterfaces[i];
			if (UMaterialInstanceConstant* MaterialInstanceConstant = Cast<UMaterialInstanceConstant>(MaterialInterface))
			{
				UMaterialInstanceDynamic* MaterialInstanceDynamic = UMaterialInstanceDynamic::Create(MaterialInstanceConstant, this);
				MaterialInstanceDynamic->SetVectorParameterValue(TEXT("Tint"), Color);
				skeletalmesh->SetMaterial(i, MaterialInstanceDynamic);
			}
			else if (UMaterialInstanceDynamic* MaterialInstanceDynamic = Cast<UMaterialInstanceDynamic>(MaterialInterface))
			{
				MaterialInstanceDynamic->SetVectorParameterValue(TEXT("Tint"), Color);
				skeletalmesh->SetMaterial(i, MaterialInstanceDynamic);
			}
		}
	}
}

bool ACJSlateCharacter::ChangeMaterialColor_Validate(FLinearColor Color)
{
	return true;
}

void ACJSlateCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	
		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void ACJSlateCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}


void ACJSlateCharacter::ShowMainMenu(const FInputActionValue& Value)
{
	bool ShowMainMenuState = Value.Get<bool>();

	if (ShowMainMenuState == true)
	{
		if (ACJSlatePlayerController* PlayerController = Cast<ACJSlatePlayerController>( GetController()))
		{
			if (bShowMainMenu == false)
			{
				PlayerController->ShowMenu();
			}
			else
			{
				PlayerController->ExitMenu();
			}
		}
	}
}

void ACJSlateCharacter::OpenInventory(const FInputActionValue& Value)
{
	bool InventoryOpenButtonState = Value.Get<bool>();

	if (InventoryOpenButtonState)
	{
		if (ACJSlatePlayerController* PlayerController = Cast<ACJSlatePlayerController>( GetController()))
		{
			if (bShowMainMenu == false)
			{
				bShowMainMenu = true;
				PlayerController->OpenInventory();
			}
			else
			{
				bShowMainMenu = false;
				PlayerController->CloseInventory();
			}
		}
	}
}

void ACJSlateCharacter::OnCharacterComponentEndOverlap(UPrimitiveComponent* OverlappedComponent,
                                                       AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (AItemActor* ItemActor = Cast<AItemActor>(OtherActor))
	{
		if (IsLocallyControlled())
			ItemActor->HideWidget();
	}
}

void ACJSlateCharacter::OnCharacterComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent,
																AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
																const FHitResult& SweepResult)
{
	if (AItemActor* ItemActor = Cast<AItemActor>(OtherActor))
	{
		if (IsLocallyControlled())
			ItemActor->ShowWidget();
	}
}
