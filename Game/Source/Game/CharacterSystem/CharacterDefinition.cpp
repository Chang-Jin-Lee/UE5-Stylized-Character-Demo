#include "Game/CharacterSystem/CharacterDefinition.h"

UCharacterDefinition::UCharacterDefinition() = default;

/*
 * @brief : PrimaryAssetId 반환
 * @details : 명시적 타입명 "CharacterDef"로 전역 충돌을 방지
 */
FPrimaryAssetId UCharacterDefinition::GetPrimaryAssetId() const
{
	static const FPrimaryAssetType CharacterType = TEXT("CharacterDef");
	return FPrimaryAssetId(CharacterType, GetFName());
}


