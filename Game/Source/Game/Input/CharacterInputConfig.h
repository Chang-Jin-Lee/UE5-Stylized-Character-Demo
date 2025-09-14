 #pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "InputCoreTypes.h"
#include "CharacterInputConfig.generated.h"

class UInputAction;
class UInputMappingContext;

USTRUCT(BlueprintType)
struct FAttackActionBinding
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSoftObjectPtr<UInputAction> Action;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FName AttackSetName = NAME_None;
};

USTRUCT(BlueprintType)
struct FSkillActionBinding
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSoftObjectPtr<UInputAction> Action;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FName SkillName = NAME_None;
};

USTRUCT(BlueprintType)
struct FActionKeyBinding
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSoftObjectPtr<UInputAction> Action;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FKey Key;
};

/*
 * @brief : 캐릭터 입력 매핑 데이터(액션→공격세트/스킬 이름, 매핑 컨텍스트)
 */
UCLASS(BlueprintType)
class GAME_API UCharacterInputConfig : public UPrimaryDataAsset
{
	GENERATED_BODY()
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Input")
	TSoftObjectPtr<UInputMappingContext> MappingContext;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Input")
	int32 MappingPriority = 1;

	// IMC를 사용하지 않고도 키를 직접 정의하고 싶을 때 사용
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Input")
	TArray<FActionKeyBinding> KeyBindings;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Input")
	TArray<FAttackActionBinding> AttackBindings;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Input")
	TArray<FSkillActionBinding> SkillBindings;
};
