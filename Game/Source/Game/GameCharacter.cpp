// Copyright Epic Games, Inc. All Rights Reserved.

#include "GameCharacter.h"
#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "Combat/CharacterCombatComponent.h"
#include "Ability/AbilityComponent.h"
#include "Game/CharacterSystem/CharacterSystemSettings.h"
#include "Game/Input/CharacterInputConfig.h"
#include "InputMappingContext.h"
#include "InputCoreTypes.h"
#include "Kismet/GameplayStatics.h"

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

//////////////////////////////////////////////////////////////////////////
// AGameCharacter

AGameCharacter::AGameCharacter()
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
	BaseWalkSpeed = GetCharacterMovement()->MaxWalkSpeed;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Combat component
	CombatComponent = CreateDefaultSubobject<UCharacterCombatComponent>(TEXT("CombatComponent"));

	// Ability component
	AbilityComponent = CreateDefaultSubobject<UAbilityComponent>(TEXT("AbilityComponent"));

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)
}

//////////////////////////////////////////////////////////////////////////
// Tick

void AGameCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	if (bAutoRestartOnFall && !bPendingRestart)
	{
		if (GetActorLocation().Z < RestartZThreshold)
		{
			bPendingRestart = true;
			UGameplayStatics::OpenLevel(this, *GetWorld()->GetName());
		}
	}
}

//////////////////////////////////////////////////////////////////////////
// Input

void AGameCharacter::NotifyControllerChanged()
{
	Super::NotifyControllerChanged();

	// Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);

			if (const UCharacterSystemSettings* Settings = GetDefault<UCharacterSystemSettings>())
			{
				if (Settings->DefaultInputConfig.IsValid())
				{
					const UCharacterInputConfig* InputCfg = Settings->DefaultInputConfig.LoadSynchronous();
					if (InputCfg)
					{
						if (InputCfg->MappingContext.IsValid())
						{
							Subsystem->AddMappingContext(InputCfg->MappingContext.LoadSynchronous(), InputCfg->MappingPriority);
						}
						// Optional direct key bindings via a runtime IMC
						if (InputCfg->KeyBindings.Num() > 0 || SprintAction)
						{
							UInputMappingContext* RuntimeIMC = NewObject<UInputMappingContext>(this, TEXT("RuntimeInputConfig"));
							for (const FActionKeyBinding& KB : InputCfg->KeyBindings)
							{
								if (KB.Action.IsValid() && KB.Key.IsValid())
								{
									UInputAction* IA = KB.Action.LoadSynchronous();
									if (IA)
									{
										RuntimeIMC->MapKey(IA, KB.Key);
									}
								}
							}
							if (SprintAction)
							{
								RuntimeIMC->MapKey(SprintAction, EKeys::LeftShift);
							}
							Subsystem->AddMappingContext(RuntimeIMC, InputCfg->MappingPriority + 1);
						}
					}
				}
			}
		}
	}
}

void AGameCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
		
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AGameCharacter::Move);


		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AGameCharacter::Look);

		// Zoom (optional direct)
		if (ZoomWheelAction)
		{
			EnhancedInputComponent->BindAction(ZoomWheelAction, ETriggerEvent::Triggered, this, &AGameCharacter::OnZoomWheel);
		}
		if (ZoomDragAction)
		{
			EnhancedInputComponent->BindAction(ZoomDragAction, ETriggerEvent::Started, this, &AGameCharacter::OnZoomDragStart);
			EnhancedInputComponent->BindAction(ZoomDragAction, ETriggerEvent::Triggered, this, &AGameCharacter::OnZoomDrag);
			EnhancedInputComponent->BindAction(ZoomDragAction, ETriggerEvent::Completed, this, &AGameCharacter::OnZoomDragEnd);
		}

		// Sprint
		if (SprintAction)
		{
			EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Started, this, &AGameCharacter::OnDashPressed);
			EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Completed, this, &AGameCharacter::OnDashReleased);
		}

		// Dynamic action bindings from InputConfig
		if (const UCharacterSystemSettings* Settings = GetDefault<UCharacterSystemSettings>())
		{
			const UCharacterInputConfig* InputCfg = Settings->DefaultInputConfig.IsValid() ? Settings->DefaultInputConfig.LoadSynchronous() : nullptr;
			if (InputCfg && CombatComponent)
			{
				for (const FAttackActionBinding& B : InputCfg->AttackBindings)
				{
					if (B.Action.IsValid())
					{
						UInputAction* IA = B.Action.LoadSynchronous();
						EnhancedInputComponent->BindAction<UCharacterCombatComponent, FName>(IA, ETriggerEvent::Started, CombatComponent, &UCharacterCombatComponent::Input_Attack, B.AttackSetName);
					}
				}
				for (const FSkillActionBinding& B : InputCfg->SkillBindings)
				{
					if (B.Action.IsValid())
					{
						UInputAction* IA = B.Action.LoadSynchronous();
						EnhancedInputComponent->BindAction<UCharacterCombatComponent, FName>(IA, ETriggerEvent::Started, CombatComponent, &UCharacterCombatComponent::Input_Skill, B.SkillName);
					}
				}
			}
		}
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void AGameCharacter::Move(const FInputActionValue& Value)
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

void AGameCharacter::Look(const FInputActionValue& Value)
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

void AGameCharacter::OnZoomWheel(const FInputActionValue& Value)
{
	const float Axis = Value.Get<float>();
	if (CameraBoom)
	{
		CameraBoom->TargetArmLength = FMath::Clamp(CameraBoom->TargetArmLength - Axis * ZoomSpeed, ZoomMin, ZoomMax);
	}
}

void AGameCharacter::OnZoomDragStart(const FInputActionValue& Value)
{
	bZoomDragging = true;
	ZoomDragPrev = Value.Get<FVector2D>();
}

void AGameCharacter::OnZoomDrag(const FInputActionValue& Value)
{
	if (!bZoomDragging || !CameraBoom) return;
	const FVector2D Curr = Value.Get<FVector2D>();
	const float DeltaY = Curr.Y - ZoomDragPrev.Y;
	ZoomDragPrev = Curr;
	CameraBoom->TargetArmLength = FMath::Clamp(CameraBoom->TargetArmLength + DeltaY * ZoomSpeed, ZoomMin, ZoomMax);
}

void AGameCharacter::OnZoomDragEnd(const FInputActionValue& Value)
{
	bZoomDragging = false;
}

void AGameCharacter::OnDashPressed()
{
	if (UCharacterMovementComponent* Move = GetCharacterMovement())
	{
		BaseWalkSpeed = Move->MaxWalkSpeed;
		Move->MaxWalkSpeed = BaseWalkSpeed * SprintMultiplier;
		bDashing = true;
	}
}

void AGameCharacter::OnDashReleased()
{
	if (UCharacterMovementComponent* Move = GetCharacterMovement())
	{
		Move->MaxWalkSpeed = BaseWalkSpeed;
		bDashing = false;
	}
}
