#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "CharacterSystemSettings.generated.h"

class UCharacterDefinition;

/*
 * @brief : 캐릭터 시스템 프로젝트 설정
 * @details :
 * 	- 기본 캐릭터 정의와 에셋 검색 경로를 노출
 * 	- 디자이너가 코드 개입 없이 설정 변경 가능
 */
UCLASS(Config=Game, DefaultConfig, meta = (DisplayName = "Character System Settings"))
class GAME_API UCharacterSystemSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:
	UCharacterSystemSettings();

	// Settings 등록 카테고리
	virtual FName GetCategoryName() const override { return TEXT("Project"); }
	virtual FName GetSectionName() const override { return TEXT("Character System"); }

	/** 기본 캐릭터 정의 (빈 경우 첫 번째 에셋을 사용하거나 폴백) */
	UPROPERTY(EditAnywhere, Config, Category = "Defaults", meta=(Tooltip="게임 시작 시 기본으로 사용할 캐릭터 정의"))
	TSoftObjectPtr<UCharacterDefinition> DefaultCharacter;

	/** PrimaryDataAsset 스캔 경로 (Content 루트 기준) */
	UPROPERTY(EditAnywhere, Config, Category = "Asset Scan", meta=(Tooltip="캐릭터 정의 에셋을 검색할 경로 목록"))
	TArray<FDirectoryPath> CharacterAssetDirectories;
};


