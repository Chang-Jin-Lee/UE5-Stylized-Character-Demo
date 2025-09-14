#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "EnemyCharacter.generated.h"

class UEnemyDefinition;
class UCharacterCombatComponent;
class AEnemyAIController;
class UAbilityComponent;

/*
 * @brief : 적 캐릭터(전투 컴포넌트 보유)
 */
UCLASS()
class GAME_API AEnemyCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AEnemyCharacter();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Spec")
	TSoftObjectPtr<UEnemyDefinition> EnemySpec;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Combat", meta=(AllowPrivateAccess="true"))
	UCharacterCombatComponent* CombatComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Ability", meta=(AllowPrivateAccess="true"))
	UAbilityComponent* AbilityComponent;

protected:
	virtual void BeginPlay() override;
};


