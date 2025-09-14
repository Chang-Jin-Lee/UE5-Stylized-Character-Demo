#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "CharacterSystemSettings.generated.h"

class UCharacterDefinition;

/*
 * @brief : ĳ���� �ý��� ������Ʈ ����
 * @details :
 * 	- �⺻ ĳ���� ���ǿ� ���� �˻� ��θ� ����
 * 	- �����̳ʰ� �ڵ� ���� ���� ���� ���� ����
 */
UCLASS(Config=Game, DefaultConfig, meta = (DisplayName = "Character System Settings"))
class GAME_API UCharacterSystemSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:
	UCharacterSystemSettings();

	// Settings ��� ī�װ�
	virtual FName GetCategoryName() const override { return TEXT("Project"); }
	virtual FName GetSectionName() const override { return TEXT("Character System"); }

	/** �⺻ ĳ���� ���� (�� ��� ù ��° ������ ����ϰų� ����) */
	UPROPERTY(EditAnywhere, Config, Category = "Defaults", meta=(Tooltip="���� ���� �� �⺻���� ����� ĳ���� ����"))
	TSoftObjectPtr<UCharacterDefinition> DefaultCharacter;

	/** PrimaryDataAsset ��ĵ ��� (Content ��Ʈ ����) */
	UPROPERTY(EditAnywhere, Config, Category = "Asset Scan", meta=(Tooltip="ĳ���� ���� ������ �˻��� ��� ���"))
	TArray<FDirectoryPath> CharacterAssetDirectories;
};


