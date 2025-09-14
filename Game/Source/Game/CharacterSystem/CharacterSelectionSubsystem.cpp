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
 * @brief : 서브시스템 초기화
 * @details : 캐릭터 에셋을 스캔하고 저장된 선택을 복원
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
 * @brief : 캐릭터 정의 에셋 스캔
 * @details : PrimaryAssetType(CharacterDef) 기준으로 전체 목록 캐시
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
 * @brief : PrimaryAssetId 문자열로 캐릭터 선택
 * @details : 동기 로드 후 브로드캐스트 및 저장
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
 * @brief : 저장된 선택 복원
 * @details : SaveGame → 실패 시 설정의 기본 캐릭터로 폴백
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
			// 직접 로드 후 선택 적용
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
 * @brief : 선택한 캐릭터 저장
 * @details : PrimaryAssetId 문자열로 저장하여 리네임/이동에도 안전
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

	// Mesh/Anim 교체 (가능한 경우)
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

		// 상대 트랜스폼 적용
		MeshComp->SetRelativeLocation(Def->MeshRelativeLocation);
		MeshComp->SetRelativeRotation(Def->MeshRelativeRotation);
		MeshComp->SetRelativeScale3D(Def->MeshRelativeScale3D);
	}

	// 캡슐 사이즈
	if (Character && Character->GetCapsuleComponent())
	{
		Character->GetCapsuleComponent()->InitCapsuleSize(Def->CapsuleRadius, Def->CapsuleHalfHeight);
	}

	// 이동 스펙
	if (Character && Character->GetCharacterMovement())
	{
		Character->GetCharacterMovement()->MaxWalkSpeed = Def->MaxWalkSpeed;
		Character->GetCharacterMovement()->JumpZVelocity = Def->JumpZVelocity;
		Character->GetCharacterMovement()->bOrientRotationToMovement = Def->bOrientRotationToMovement;
	}

	// 카메라 붐 길이 (선택)
	if (Def->CameraBoomArmLength > 0.f && Character)
	{
		TArray<USpringArmComponent*> Arms;
		Character->GetComponents(Arms);
		for (USpringArmComponent* Arm : Arms)
		{
			Arm->TargetArmLength = Def->CameraBoomArmLength;
		}
	}

	// 전투 스펙 적용
	ApplyCombatSpecIfAny(Pawn, Def);
}

/*
 * @brief : 현재 선택 캐릭터로 플레이어 스폰/스왑
 * @details : 선택 PawnClass 없으면 GameMode의 기본 PawnClass로 폴백, Possess 교체
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

	// 클래스가 동일하고 스펙만 바꾸면 되는 경우: 동적 적용 후 반환
	if (OldPawn && (!PawnClass || OldPawn->IsA(PawnClass)))
	{
		ApplyCharacterSpec(OldPawn, Def);
		return OldPawn;
	}

	if (!PawnClass)
	{
		return OldPawn; // 최종 폴백: 기존 Pawn 유지
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


