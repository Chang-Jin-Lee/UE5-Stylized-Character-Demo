#include "Game/Player/GamePlayerController.h"
#include "Game/CharacterSystem/CharacterSelectionSubsystem.h"
#include "Engine/World.h"
#include "GameFramework/PlayerState.h"

/*
 * @brief : 시작 시 저장된 캐릭터로 스폰/스왑
 * @details : GameInstanceSubsystem을 통해 선택값을 복원하고 Pawn 교체
 */
void AGamePlayerController::BeginPlay()
{
	Super::BeginPlay();
	if (UCharacterSelectionSubsystem* Subsystem = GetCharacterSubsystem())
	{
		// 첫 진입 시 저장된 캐릭터로 스폰/스왑
		Subsystem->SpawnOrSwapPlayer(GetWorld(), this);
	}
}

/*
 * @brief : 캐릭터 서브시스템 접근자
 * @details : GameInstance로부터 Subsystem 획득
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
 * @brief : PrimaryAssetId 문자열로 캐릭터 선택 요청
 * @details : 선택 적용 후 즉시 스폰/스왑
 */
void AGamePlayerController::RequestSelectCharacter(const FString& PrimaryAssetIdString)
{
	if (UCharacterSelectionSubsystem* Subsystem = GetCharacterSubsystem())
	{
		Subsystem->SelectCharacterById(PrimaryAssetIdString);
		Subsystem->SpawnOrSwapPlayer(GetWorld(), this);
	}
}


