#include "Game/CharacterSystem/CharacterSelectionSubsystem.h"
#include "Game/CharacterSystem/CharacterDefinition.h"
#include "Game/CharacterSystem/CharacterSystemSettings.h"
#include "Game/CharacterSystem/CharacterSaveGame.h"
#include "Engine/AssetManager.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/GameModeBase.h"
#include "GameFramework/Character.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Game/Combat/CharacterCombatComponent.h"

static const FString SaveSlotName = TEXT("CharacterSelection");
static const uint32 SaveUserIndex = 0;

/*
 * @brief : ����ý��� �ʱ�ȭ
 * @details : ĳ���� ������ ��ĵ�ϰ� ����� ������ ����
 */
void UCharacterSelectionSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	ScanCharacterAssets();
	LoadSavedSelection();
}

void UCharacterSelectionSubsystem::Deinitialize()
{
	Super::Deinitialize();
}

/*
 * @brief : ĳ���� ���� ���� ��ĵ
 * @details : PrimaryAssetType(CharacterDef) �������� ��ü ��� ĳ��
 */
void UCharacterSelectionSubsystem::ScanCharacterAssets()
{
	CachedCharacterAssetIds.Reset();
	const UCharacterSystemSettings* Settings = GetDefault<UCharacterSystemSettings>();
	UAssetManager& AM = UAssetManager::Get();

	const FPrimaryAssetType Type = TEXT("CharacterDef");
	AM.GetPrimaryAssetIdList(Type, CachedCharacterAssetIds);
}

/*
 * @brief : PrimaryAssetId ���ڿ��� ĳ���� ����
 * @details : ���� �ε� �� ��ε�ĳ��Ʈ �� ����
 */
void UCharacterSelectionSubsystem::SelectCharacterById(const FString& PrimaryAssetIdString)
{
	FPrimaryAssetId Id = FPrimaryAssetId::ParseTypeAndName(PrimaryAssetIdString);
	if (!Id.IsValid()) return;

	UAssetManager& AM = UAssetManager::Get();
	const FSoftObjectPath AssetPath = AM.GetPrimaryAssetPath(Id);
	if (!AssetPath.IsValid()) return;

	Streamable.RequestSyncLoad(AssetPath);
	UCharacterDefinition* Def = Cast<UCharacterDefinition>(AssetPath.ResolveObject());
	if (Def)
	{
		SelectedCharacter = Def;
		OnCharacterSelected.Broadcast(Def);
		SaveSelection(Id);
	}
}

void UCharacterSelectionSubsystem::GetAllCharacterIds(TArray<FString>& OutIds) const
{
	OutIds.Reset();
	OutIds.Reserve(CachedCharacterAssetIds.Num());
	for (const FPrimaryAssetId& Id : CachedCharacterAssetIds)
	{
		OutIds.Add(Id.ToString());
	}
}

/*
 * @brief : ����� ���� ����
 * @details : SaveGame �� ���� �� ������ �⺻ ĳ���ͷ� ����
 */
void UCharacterSelectionSubsystem::LoadSavedSelection()
{
	if (USaveGame* Loaded = UGameplayStatics::LoadGameFromSlot(SaveSlotName, SaveUserIndex))
	{
		if (const UCharacterSaveGame* Save = Cast<UCharacterSaveGame>(Loaded))
		{
			SelectCharacterById(Save->SelectedCharacterId);
			return;
		}
	}

	const UCharacterSystemSettings* Settings = GetDefault<UCharacterSystemSettings>();
	if (Settings)
	{
		const FSoftObjectPath SoftPath = Settings->DefaultCharacter.ToSoftObjectPath();
		if (SoftPath.IsValid())
		{
			UAssetManager& AM = UAssetManager::Get();
			const FPrimaryAssetId DefaultId = AM.GetPrimaryAssetIdForPath(SoftPath);
			if (DefaultId.IsValid())
			{
				SelectCharacterById(DefaultId.ToString());
				return;
			}
			// ���� �ε� �� ���� ����
			Streamable.RequestSyncLoad(SoftPath);
			if (UCharacterDefinition* Def = Cast<UCharacterDefinition>(SoftPath.ResolveObject()))
			{
				SelectedCharacter = Def;
				OnCharacterSelected.Broadcast(Def);
			}
		}
	}
}

/*
 * @brief : ������ ĳ���� ����
 * @details : PrimaryAssetId ���ڿ��� �����Ͽ� ������/�̵����� ����
 */
void UCharacterSelectionSubsystem::SaveSelection(const FPrimaryAssetId& AssetId)
{
	UCharacterSaveGame* Save = Cast<UCharacterSaveGame>(UGameplayStatics::CreateSaveGameObject(UCharacterSaveGame::StaticClass()));
	if (!Save) return;
	Save->SelectedCharacterId = AssetId.ToString();
	UGameplayStatics::SaveGameToSlot(Save, SaveSlotName, SaveUserIndex);
}

static void ApplyCombatSpecIfAny(APawn* Pawn, const UCharacterDefinition* Def)
{
	if (!Pawn || !Def) return;
	if (UCharacterCombatComponent* Combat = Pawn->FindComponentByClass<UCharacterCombatComponent>())
	{
		// Fill basic fields via component API for future extension
		Combat->ApplyCombatSpec(Def);
	}
}

static void ApplyCharacterSpec(APawn* Pawn, const UCharacterDefinition* Def)
{
	if (!Pawn || !Def) return;

	ACharacter* Character = Cast<ACharacter>(Pawn);
	USkeletalMeshComponent* MeshComp = Character ? Character->GetMesh() : nullptr;

	// Mesh/Anim ��ü (������ ���)
	if (MeshComp)
	{
		if (Def->Mesh.IsValid())
		{
			USkeletalMesh* Mesh = Def->Mesh.LoadSynchronous();
			if (Mesh)
			{
				MeshComp->SetSkeletalMesh(Mesh);
			}
		}
		if (Def->AnimClass.IsValid())
		{
			UClass* AnimCls = Def->AnimClass.LoadSynchronous();
			if (AnimCls)
			{
				MeshComp->SetAnimInstanceClass(AnimCls);
			}
		}

		// ��� Ʈ������ ����
		MeshComp->SetRelativeLocation(Def->MeshRelativeLocation);
		MeshComp->SetRelativeRotation(Def->MeshRelativeRotation);
		MeshComp->SetRelativeScale3D(Def->MeshRelativeScale3D);
	}

	// ĸ�� ������
	if (Character && Character->GetCapsuleComponent())
	{
		Character->GetCapsuleComponent()->InitCapsuleSize(Def->CapsuleRadius, Def->CapsuleHalfHeight);
	}

	// �̵� ����
	if (Character && Character->GetCharacterMovement())
	{
		Character->GetCharacterMovement()->MaxWalkSpeed = Def->MaxWalkSpeed;
		Character->GetCharacterMovement()->JumpZVelocity = Def->JumpZVelocity;
		Character->GetCharacterMovement()->bOrientRotationToMovement = Def->bOrientRotationToMovement;
	}

	// ī�޶� �� ���� (����)
	if (Def->CameraBoomArmLength > 0.f && Character)
	{
		TArray<USpringArmComponent*> Arms;
		Character->GetComponents(Arms);
		for (USpringArmComponent* Arm : Arms)
		{
			Arm->TargetArmLength = Def->CameraBoomArmLength;
		}
	}

	// ���� ���� ����
	ApplyCombatSpecIfAny(Pawn, Def);
}

/*
 * @brief : ���� ���� ĳ���ͷ� �÷��̾� ����/����
 * @details : ���� PawnClass ������ GameMode�� �⺻ PawnClass�� ����, Possess ��ü
 */
APawn* UCharacterSelectionSubsystem::SpawnOrSwapPlayer(UWorld* World, AController* Controller)
{
	if (!World || !Controller) return nullptr;

	APawn* OldPawn = Controller->GetPawn();

	UCharacterDefinition* Def = SelectedCharacter.Get();
	TSubclassOf<APawn> PawnClass = nullptr;
	if (Def && Def->PawnClass.IsValid())
	{
		PawnClass = Def->PawnClass.LoadSynchronous();
	}

	if (!PawnClass)
	{
		if (AGameModeBase* GameMode = World->GetAuthGameMode<AGameModeBase>())
		{
			PawnClass = GameMode->GetDefaultPawnClassForController(Controller);
		}
	}

	// Ŭ������ �����ϰ� ���常 �ٲٸ� �Ǵ� ���: ���� ���� �� ��ȯ
	if (OldPawn && (!PawnClass || OldPawn->IsA(PawnClass)))
	{
		ApplyCharacterSpec(OldPawn, Def);
		return OldPawn;
	}

	if (!PawnClass)
	{
		return OldPawn; // ���� ����: ���� Pawn ����
	}

	FVector Location = FVector::ZeroVector;
	FRotator Rotation = FRotator::ZeroRotator;
	if (APlayerController* PC = Cast<APlayerController>(Controller))
	{
		PC->GetPlayerViewPoint(Location, Rotation);
	}

	FActorSpawnParameters Params;
	Params.Owner = Controller;
	Params.Instigator = OldPawn;
	APawn* NewPawn = World->SpawnActor<APawn>(PawnClass, Location, Rotation, Params);

	if (OldPawn)
	{
		Controller->UnPossess();
		OldPawn->Destroy();
	}

	if (NewPawn)
	{
		Controller->Possess(NewPawn);
		ApplyCharacterSpec(NewPawn, Def);
	}
	return NewPawn ? NewPawn : OldPawn;
}


