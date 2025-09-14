#include "Game/CharacterSystem/CharacterSystemSettings.h"

UCharacterSystemSettings::UCharacterSystemSettings()
{
	CategoryName = TEXT("Project");
	SectionName = TEXT("Character System");

	// 기본 스캔 경로 예시: /Game/Characters/Definitions
	FDirectoryPath DefaultDir;
	DefaultDir.Path = TEXT("/Game/Characters");
	CharacterAssetDirectories.Add(DefaultDir);
}


