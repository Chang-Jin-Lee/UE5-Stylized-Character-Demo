#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "CharacterSaveGame.generated.h"

/*
 * @brief : 선택 캐릭터 SaveGame
 * @details : PrimaryAssetId 문자열(Type:Name)로 저장해 리네임/이동에도 안전
 */
UCLASS()
class GAME_API UCharacterSaveGame : public USaveGame
{
	GENERATED_BODY()

public:
	// Primary Asset Id 문자열 표현 (Type:Name)
	UPROPERTY()
	FString SelectedCharacterId;
};


