
#include "Characters/PlayerCharacter.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/InputComponent.h"
#include "InputMappingContext.h"
#include "Characters/NonPlayerCharacter.h"
#include "Components/CapsuleComponent.h"
#include "Components/InventoryComponent.h"
#include "Components/TextBlock.h"
#include "Components/WidgetComponent.h"
#include "Items/Base_Weapon.h"
#include "Items/Katana.h"
#include "Items/LongSword.h"
#include "Kismet/GameplayStatics.h"
#include "Widgets/InventoryUIWidget.h"
#include "Widgets/NameTagWidget.h"

// Sets default values
APlayerCharacter::APlayerCharacter()
{
	// Set this character to call Tick() every frame. You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
	
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	CameraArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraArm"));
	CameraArm->SetupAttachment(RootComponent);
	CameraArm->TargetArmLength = 600.0f;
	CameraArm->bUsePawnControlRotation = true;

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	CameraComponent->SetupAttachment(CameraArm, USpringArmComponent::SocketName);
	CameraComponent->bUsePawnControlRotation = false;

	// Configure character movement
	GetCharacterMovement()->MaxWalkSpeed = 500.f;

	static ConstructorHelpers::FObjectFinder<UInputMappingContext> FindMappingContext(TEXT("InputMappingContext'/Game/Inputs/IMC_Default.IMC_Default'"));
	if (FindMappingContext.Succeeded())
	{
		DefaultMappingContext = FindMappingContext.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> FindMoveAction(TEXT("/Script/EnhancedInput.InputAction'/Game/Inputs/Actions/IA_Move.IA_Move'"));
	if (FindMoveAction.Succeeded())
	{
		MoveAction = FindMoveAction.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> FindLookAction(TEXT("/Script/EnhancedInput.InputAction'/Game/Inputs/Actions/IA_Look.IA_Look'"));
	if (FindLookAction.Succeeded())
	{
		LookAction = FindLookAction.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> FindOpenInventoryAction(TEXT("/Script/EnhancedInput.InputAction'/Game/Inputs/Actions/IA_OpenInventory.IA_OpenInventory'"));
	if (FindOpenInventoryAction.Succeeded())
	{
		OpenInventoryAction = FindOpenInventoryAction.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimBlueprint> FindAnimInstance(TEXT("/Game/Characters/ABP_PlayerAnimation.ABP_PlayerAnimation"));
	if (FindAnimInstance.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(FindAnimInstance.Object->GeneratedClass);
	}

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> FindSkeletalMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/StarterPack/Characters/Mannequins/Meshes/SKM_Manny.SKM_Manny'"));
	if(FindSkeletalMesh.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(FindSkeletalMesh.Object);
		GetMesh()->SetRelativeLocation(FVector(0,0,-96.0f));
		GetMesh()->SetRelativeRotation(FRotator(0,-90,0));
	}

	// NameTagWidget 생성 및 초기화
	NameTagWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("NameTagWidget"));
	NameTagWidgetComponent->SetupAttachment(RootComponent);
	NameTagWidgetComponent->SetWidgetSpace(EWidgetSpace::Screen); // 스크린 공간에서 위젯 표시
	NameTagWidgetComponent->SetRelativeLocation(FVector(0.f, 0.f, 150.f)); // 캐릭터 머리 위에 위치
	// NameTagWidgetComponent->SetDrawSize(FVector2D(250.f, 100.f)); // 위젯의 크기 설정
	
	UNameTagWidget* NameTagWidget = CreateWidget<UNameTagWidget>(GetWorld(), UNameTagWidget::StaticClass());
	if(NameTagWidget)
	{
		NameTagWidgetComponent->SetWidget(NameTagWidget);
	}
	
	NameTagWidgetComponent->SetVisibility(false); // 초기에는 보이지 않도록 설정
	
	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &APlayerCharacter::OnOverlapBegin);
	GetCapsuleComponent()->OnComponentEndOverlap.AddDynamic(this, &APlayerCharacter::OnOverlapEnd);


	InventoryWidgetPosition = FVector2D(1300, 250);
	InventoryGridCount = FVector2D(6,4);
	InventoryComponent->Items.SetNum(InventoryGridCount.X * InventoryGridCount.Y);
	for(int32 i = 0; i < InventoryGridCount.X * InventoryGridCount.Y; i++)
	{
		InventoryComponent->Items[i].Empty();
	}
}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
    Super::BeginPlay();

	//Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
	
	CreateNameInputWidget();

	FInventoryItem DefaultSword;
	DefaultSword.TestSword1();
	DefaultSword.ItemClass = AKatana::StaticClass();
	DrawWeapon(DefaultSword);
}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Move);
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Look);
		EnhancedInputComponent->BindAction(OpenInventoryAction, ETriggerEvent::Triggered, this, &APlayerCharacter::OpenInventory);
	}
}

void APlayerCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// Get the current control rotation (which way the camera is facing)
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// Get forward and right direction vectors based on the camera's rotation
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// Calculate the movement direction
		FVector MovementDirection = (ForwardDirection * MovementVector.Y) + (RightDirection * MovementVector.X);
		MovementDirection.Normalize(); 
		AddMovementInput(MovementDirection);

		if (!MovementDirection.IsNearlyZero())
		{
			FRotator TargetRotation = MovementDirection.Rotation();
			FRotator AdjustedRotation = TargetRotation + FRotator(0.0f, -90.0f, 0.0f); // Y축으로 -90도 보정
			GetMesh()->SetWorldRotation(AdjustedRotation);
		}
	}
}

void APlayerCharacter::Look(const FInputActionValue& Value)
{
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void APlayerCharacter::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && (OtherActor != this) && OtherComp)
	{
		if(OtherActor->IsA<ANonPlayerCharacter>())
		{
			ANonPlayerCharacter* npc = Cast<ANonPlayerCharacter>(OtherActor);
			if(npc)
			{
				NonPlayerCharacter = npc;
				npc->PlayerCharacer = this;
				OpenInventory();
				npc->OpenNpcInventory();
			}
		}
	}
}

void APlayerCharacter::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor && (OtherActor != this) && OtherComp)
	{
		ANonPlayerCharacter* npc = Cast<ANonPlayerCharacter>(OtherActor);
		if(npc)
		{
			CloseInventory();
			npc->CloseInventory();
			NonPlayerCharacter = nullptr;
			npc->PlayerCharacer = nullptr;
		}
	}
}


void APlayerCharacter::CreateNameInputWidget()
{
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);
	if (PlayerController)
	{
		PlayerController->SetInputMode(FInputModeUIOnly());
		PlayerController->bShowMouseCursor = true;
	}
	
	if (!NameInputWidget)
	{
		NameInputWidget = CreateWidget<UNameInputWidget>(GetWorld(), UNameInputWidget::StaticClass());
		if (NameInputWidget)
		{
			NameInputWidget->AddToViewport();
			
			if (NameInputWidget->ConfirmButton)
			{
				NameInputWidget->ConfirmButton->OnClicked.AddDynamic(this, &APlayerCharacter::SetNameTag);
			}
		}
	}
}

void APlayerCharacter::SetNameTag()
{
	if (NameTagWidgetComponent)
	{
		FString EnteredName = NameInputWidget->NameTextBox->GetText().ToString();

		// NameTagWidget에서 UTextBlock을 가져와 텍스트를 설정
		UNameTagWidget* Widget = Cast<UNameTagWidget>(NameTagWidgetComponent->GetUserWidgetObject());
		if (Widget)
		{
			UTextBlock* TextBlock = Cast<UTextBlock>(Widget->GetWidgetFromName(TEXT("NameTextBlock")));
			if (TextBlock)
			{
				TextBlock->SetText(FText::FromString(EnteredName));
			}
		}
		NameTagWidgetComponent->GetWidget()->SetVisibility(ESlateVisibility::Visible); // 이름 태그를 표시

		// 입력 위젯 제거
		NameInputWidget->RemoveFromViewport();

		// 게임 인풋 모드 다시 설정
		APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);
		if (PlayerController)
		{
			PlayerController->SetInputMode(FInputModeGameOnly());
			PlayerController->bShowMouseCursor = false;
		}
	}
}

// 특정 이름을 가진 자식 메쉬를 찾는 함수
USkeletalMeshComponent* APlayerCharacter::FindMeshByName(FName MeshName)
{
	// 모든 SkeletalMeshComponent를 찾습니다.
	TArray<USkeletalMeshComponent*> MeshComponents;
	GetComponents<USkeletalMeshComponent>(MeshComponents);

	// 각 컴포넌트를 순회하며 이름이 일치하는 메쉬를 찾습니다.
	for (USkeletalMeshComponent* MeshComp : MeshComponents)
	{
		if (MeshComp->GetName() == MeshName.ToString())
		{
			return MeshComp;
		}
	}
	// 찾지 못하면 nullptr 반환
	return nullptr;
}

void APlayerCharacter::DrawWeaponFromInventory(int32 WeaponIndex)
{
	if(!InventoryComponent->Items[WeaponIndex].IsEmpty())
	{
		FInventoryItem Item = InventoryComponent->Items[WeaponIndex];
		if(CurrentWeapon)
		{
			CurrentWeapon->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
			CurrentWeapon->SetActorEnableCollision(false);
			CurrentWeapon->Destroy();
			CurrentWeapon = nullptr;
		}
	
		DrawWeapon(Item);
	}
}

void APlayerCharacter::DrawWeapon(FInventoryItem Itemdata)
{
	USkeletalMeshComponent* SpecificMesh = FindMeshByName(TEXT("CharacterMesh0"));
	
	if(!CurrentWeapon)
	{
		AActor* SpawnedActor = nullptr;
		
		switch (Itemdata.WeaponType)
		{
		case EWeaponType::Wt_Katana:
			SpawnedActor = GetWorld()->SpawnActor<AActor>(AKatana::StaticClass());
			break;
		case EWeaponType::Wt_LongSword:
			SpawnedActor = GetWorld()->SpawnActor<AActor>(ALongSword::StaticClass());
		default:
			break;
		}
		
		if(SpawnedActor)
		{
			CurrentWeapon = Cast<ABase_Weapon>(SpawnedActor);
			CurrentWeapon->ItemID = Itemdata.ItemID;
			CurrentWeapon->ItemName = Itemdata.ItemName;
			CurrentWeapon->ItemClass = Itemdata.ItemClass;
			CurrentWeapon->ItemImage = Itemdata.ItemImage;
			CurrentWeapon->Quantity = Itemdata.Quantity;
			CurrentWeapon->ItemDescription = Itemdata.ItemDescription;
			CurrentWeapon->ItemType = Itemdata.ItemType;
			CurrentWeapon->SetActorEnableCollision(false);

			if (CurrentWeapon)
			{
				// 액터에 연결된 모든 컴포넌트를 가져옴
				TArray<UActorComponent*> Components;
				CurrentWeapon->GetComponents(Components);

				// 모든 컴포넌트에 대해 반복하여 충돌을 비활성화
				for (UActorComponent* Component : Components)
				{
					UPrimitiveComponent* PrimitiveComponent = Cast<UPrimitiveComponent>(Component);
					if (PrimitiveComponent)
					{
						PrimitiveComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
					}
				}
			}
			
			CurrentWeapon->SetActorRelativeLocation(FVector(-6.055225,2.567933, -13.115323));
			CurrentWeapon->SetActorRelativeRotation(FRotator(1.179432,90.546514,-3.142943));
			CurrentWeapon->AttachToComponent(SpecificMesh, FAttachmentTransformRules::KeepRelativeTransform, TEXT("Weapon_r"));
			CurrentWeapon->SetOwner(this);
		}
	}
}
