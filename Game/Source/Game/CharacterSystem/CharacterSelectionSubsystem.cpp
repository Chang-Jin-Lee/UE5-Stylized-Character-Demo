#include "Game/CharacterSystem/CharacterSelectionSubsystem.h"
#include "Game/CharacterSystem/CharacterDefinition.h"
#include "Game/CharacterSystem/CharacterSystemSettings.h"
#include "Game/CharacterSystem/CharacterSaveGame.h"
#include "Engine/AssetManager.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/GameModeBase.h"

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
	if (Settings && Settings->DefaultCharacter.IsValid())
	{
		SelectCharacterById(Settings->DefaultCharacter.ToSoftObjectPath().GetAssetPathString());
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

	// Ŭ������ �����ϸ� ���� ���ʿ�
	if (OldPawn && PawnClass && OldPawn->IsA(PawnClass))
	{
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
	}
	return NewPawn ? NewPawn : OldPawn;
}


