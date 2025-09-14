#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "AbilitySettings.generated.h"

/*
 * @brief : ���� �ɷ�ġ �⺻�� ����(������ Project Settings ����)
 */
UCLASS(Config=Game, DefaultConfig, meta=(DisplayName="Ability Settings"))
class GAME_API UAbilitySettings : public UDeveloperSettings
{
	GENERATED_BODY()
public:
	virtual FName GetCategoryName() const override { return TEXT("Project"); }
	virtual FName GetSectionName() const override { return TEXT("Ability"); }

	// �⺻ �ɷ�ġ Ű-�� (��: Health=100, Attack=20, Defense=5, HealPower=10)
	UPROPERTY(EditAnywhere, Config, Category="Defaults")
	TMap<FName, float> DefaultAttributes;
};


