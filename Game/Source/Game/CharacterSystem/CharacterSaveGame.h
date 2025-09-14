#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "CharacterSaveGame.generated.h"

/*
 * @brief : ���� ĳ���� SaveGame
 * @details : PrimaryAssetId ���ڿ�(Type:Name)�� ������ ������/�̵����� ����
 */
UCLASS()
class GAME_API UCharacterSaveGame : public USaveGame
{
	GENERATED_BODY()

public:
	// Primary Asset Id ���ڿ� ǥ�� (Type:Name)
	UPROPERTY()
	FString SelectedCharacterId;
};


