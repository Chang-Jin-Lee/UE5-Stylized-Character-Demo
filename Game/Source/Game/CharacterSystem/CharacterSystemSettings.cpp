#include "Game/CharacterSystem/CharacterSystemSettings.h"

UCharacterSystemSettings::UCharacterSystemSettings()
{
	CategoryName = TEXT("Project");
	SectionName = TEXT("Character System");

	// �⺻ ��ĵ ��� ����: /Game/Characters/Definitions
	FDirectoryPath DefaultDir;
	DefaultDir.Path = TEXT("/Game/Characters");
	CharacterAssetDirectories.Add(DefaultDir);
}


