#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "AbilitySettings.generated.h"

/*
 * @brief : 전역 능력치 기본값 설정(에디터 Project Settings 노출)
 */
UCLASS(Config=Game, DefaultConfig, meta=(DisplayName="Ability Settings"))
class GAME_API UAbilitySettings : public UDeveloperSettings
{
	GENERATED_BODY()
public:
	virtual FName GetCategoryName() const override { return TEXT("Project"); }
	virtual FName GetSectionName() const override { return TEXT("Ability"); }

	// 기본 능력치 키-값 (예: Health=100, Attack=20, Defense=5, HealPower=10)
	UPROPERTY(EditAnywhere, Config, Category="Defaults")
	TMap<FName, float> DefaultAttributes;
};


