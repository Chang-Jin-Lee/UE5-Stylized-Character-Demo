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
 * @brief : ĳ���� �Է� ���� ������(�׼ǡ���ݼ�Ʈ/��ų �̸�, ���� ���ؽ�Ʈ)
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

	// IMC�� ������� �ʰ� Ű�� ���� �����ϰ� ���� �� ���
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Input")
	TArray<FActionKeyBinding> KeyBindings;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Input")
	TArray<FAttackActionBinding> AttackBindings;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Input")
	TArray<FSkillActionBinding> SkillBindings;
};
