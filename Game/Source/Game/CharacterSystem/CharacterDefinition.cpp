#include "Game/CharacterSystem/CharacterDefinition.h"

UCharacterDefinition::UCharacterDefinition() = default;

/*
 * @brief : PrimaryAssetId ��ȯ
 * @details : ����� Ÿ�Ը� "CharacterDef"�� ���� �浹�� ����
 */
FPrimaryAssetId UCharacterDefinition::GetPrimaryAssetId() const
{
	static const FPrimaryAssetType CharacterType = TEXT("CharacterDef");
	return FPrimaryAssetId(CharacterType, GetFName());
}


