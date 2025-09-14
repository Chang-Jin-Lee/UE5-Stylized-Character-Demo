#include "Game/Player/GamePlayerController.h"
#include "Game/CharacterSystem/CharacterSelectionSubsystem.h"
#include "Engine/World.h"
#include "GameFramework/PlayerState.h"

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


