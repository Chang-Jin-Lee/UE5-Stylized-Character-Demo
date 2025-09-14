#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Game/CharacterSystem/CharacterDefinition.h"
#include "CharacterCombatComponent.generated.h"

class UAnimMontage;
class UCharacterDefinition;

enum class EMontageSelectPolicy : uint8;

UENUM(BlueprintType)
enum class ECombatActionType : uint8
{
	Attack,
	Skill
};

/*
 * @brief : 캐릭터 전투 컴포넌트(공격/스킬/히트 판정/콤보)
 */
UCLASS(BlueprintType, Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class GAME_API UCharacterCombatComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UCharacterCombatComponent();

	UFUNCTION(BlueprintCallable, Category="Combat")
	bool TryAttack();
	UFUNCTION(BlueprintCallable, Category="Combat")
	bool TrySkill();

	UFUNCTION(BlueprintCallable, Category="Combat")
	bool TryAttackBySet(FName AttackSetName);
	UFUNCTION(BlueprintCallable, Category="Combat")
	bool TrySkillByName(FName SkillName);

	void Input_Attack(const struct FInputActionInstance& Instance, FName AttackSetName);
	void Input_Skill(const struct FInputActionInstance& Instance, FName SkillName);

	void ApplyCombatSpec(const UCharacterDefinition* Def);

	UFUNCTION(BlueprintCallable, Category="Combat")
	void ForceDoHitTrace(bool bSkill);

protected:
	virtual void BeginPlay() override;

private:
	TArray<TWeakObjectPtr<UAnimMontage>> AttackMontages;
	TArray<TWeakObjectPtr<UAnimMontage>> SkillMontages;
	EMontageSelectPolicy AttackPolicy;
	EMontageSelectPolicy SkillPolicy;
	int32 AttackSeqIndex = 0;
	int32 SkillSeqIndex = 0;

	TMap<FName, FAttackComboSpec> NameToCombo;
	TMap<FName, FSkillSpec> NameToSkill;

	FName CurrentAttackSet = NAME_None;
	int32 CurrentStageIndex = 0; // 0-based
	double ComboStartTime = -1.0;
	double LastComboEndTime = -1.0;

	float AttackCooldown = 0.6f;
	float SkillCooldown = 3.0f;
	float DamageAttack = 20.f;
	float AttackMultiplierAttack = 0.f;
	float DamageSkill = 50.f;
	float AttackMultiplierSkill = 0.f;
	float HitRadiusAttack = 30.f;
	float HitRangeAttack = 150.f;
	FName HitSocketAttack = NAME_None;
	float HitRadiusSkill = 60.f;
	float HitRangeSkill = 250.f;
	FName HitSocketSkill = NAME_None;

	FName AttackNotifyBegin = TEXT("AttackStart");
	FName AttackNotifyEnd   = TEXT("AttackEnd");
	FName SkillNotifyBegin  = TEXT("SkillStart");
	FName SkillNotifyEnd    = TEXT("SkillEnd");
	FName ComboWindowOpen   = TEXT("ComboWindowOpen");
	FName ComboWindowClose  = TEXT("ComboWindowClose");
	bool bComboInputWindow = false;

	// 콤보 입력 큐(빠른 연타 흡수). 창 밖 입력도 누적.
	int32 QueuedComboCount = 0;
	double LastAttackInputTime = -1.0; // 디버그용
	float InputGraceSeconds = 0.15f;   // 필요시 조정

	double LastAttackTime = -1.0;
	double LastSkillTime  = -1.0;
	bool bAttackWindow = false;
	bool bSkillWindow = false;

	bool CanUse(double LastTime, float CooldownSec) const;
	UAnimMontage* PickMontage(TArray<TWeakObjectPtr<UAnimMontage>>& List, EMontageSelectPolicy Policy, int32& SeqIndex) const;
	bool PlayMontage(UAnimMontage* Montage);
	void BindAnimDelegates();
	UFUNCTION()
	void OnNotifyBegin(FName NotifyName, const FBranchingPointNotifyPayload& Payload);
	UFUNCTION()
	void OnNotifyEnd(FName NotifyName, const FBranchingPointNotifyPayload& Payload);
	void DoHitTrace(bool bSkill);

	bool StartOrContinueCombo(FName AttackSetName);
	bool PlayCurrentComboStage();
	void EndCombo();
};
