#include "Game/Combat/CharacterCombatComponent.h"
#include "Game/CharacterSystem/CharacterDefinition.h"
#include "GameFramework/Character.h"
#include "Components/SkeletalMeshComponent.h"
#include "Animation/AnimInstance.h"
#include "Animation/AnimMontage.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "EnhancedInputComponent.h"
#include "Game/Ability/AbilityComponent.h"

UCharacterCombatComponent::UCharacterCombatComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UCharacterCombatComponent::BeginPlay()
{
	Super::BeginPlay();
	BindAnimDelegates();
}

void UCharacterCombatComponent::Input_Attack(const FInputActionInstance& Instance, FName AttackSetName)
{
	LastAttackInputTime = UGameplayStatics::GetTimeSeconds(this);
	// �Է��� ������ �׻� ť�� 1 �������� ���� ��ȯ�� ���
	QueuedComboCount = FMath::Clamp(QueuedComboCount + 1, 0, 4);
	// �޺� ������ ���¶�� ��� ����
	if (CurrentAttackSet == NAME_None)
	{
		TryAttackBySet(AttackSetName);
	}
}

void UCharacterCombatComponent::Input_Skill(const FInputActionInstance& Instance, FName SkillName)
{
	TrySkillByName(SkillName);
}

void UCharacterCombatComponent::ApplyCombatSpec(const UCharacterDefinition* Def)
{
	if (!Def) return;

	AttackMontages.Reset();
	SkillMontages.Reset();
	NameToCombo = Def->AttackCombos;
	NameToSkill = Def->Skills;

	for (const TSoftObjectPtr<UAnimMontage>& Soft : Def->AttackMontages)
	{
		if (Soft.IsNull()) continue;
		UAnimMontage* M = Soft.LoadSynchronous();
		if (M) AttackMontages.Add(M);
	}
	for (const TSoftObjectPtr<UAnimMontage>& Soft : Def->SkillMontages)
	{
		if (Soft.IsNull()) continue;
		UAnimMontage* M = Soft.LoadSynchronous();
		if (M) SkillMontages.Add(M);
	}

	AttackPolicy = Def->AttackSelectPolicy;
	SkillPolicy  = Def->SkillSelectPolicy;
	AttackSeqIndex = 0;
	SkillSeqIndex  = 0;

	AttackCooldown = Def->AttackCooldown;
	SkillCooldown  = Def->SkillCooldown;
	DamageAttack   = Def->DamageAttack;
	DamageSkill    = Def->DamageSkill;
	AttackMultiplierAttack = 0.f;
	AttackMultiplierSkill = 0.f;
	HitRadiusAttack = Def->HitRadiusAttack;
	HitRangeAttack  = Def->HitRangeAttack;
	HitSocketAttack = Def->HitSocketAttack;
	HitRadiusSkill  = Def->HitRadiusSkill;
	HitRangeSkill   = Def->HitRangeSkill;
	HitSocketSkill  = Def->HitSocketSkill;

	CurrentAttackSet = NAME_None;
	CurrentStageIndex = 0;
	ComboStartTime = -1.0;
	LastComboEndTime = -1.0;
	bComboInputWindow = false;
	QueuedComboCount = 0;
	LastAttackInputTime = -1.0;

	//BindAnimDelegates();
}

bool UCharacterCombatComponent::CanUse(double LastTime, float CooldownSec) const
{
	const double Now = UGameplayStatics::GetTimeSeconds(this);
	return (LastTime < 0.0) || (Now - LastTime >= CooldownSec);
}

UAnimMontage* UCharacterCombatComponent::PickMontage(TArray<TWeakObjectPtr<UAnimMontage>>& List, EMontageSelectPolicy Policy, int32& SeqIndex) const
{
	if (List.Num() == 0) return nullptr;
	if (Policy == EMontageSelectPolicy::Random)
	{
		const int32 Index = FMath::RandRange(0, List.Num() - 1);
		return List[Index].IsValid() ? List[Index].Get() : nullptr;
	}
	int32 SafeIndex = SeqIndex % List.Num();
	UAnimMontage* Picked = List[SafeIndex].IsValid() ? List[SafeIndex].Get() : nullptr;
	SeqIndex = (SafeIndex + 1) % List.Num();
	return Picked;
}

bool UCharacterCombatComponent::PlayMontage(UAnimMontage* Montage)
{
	ACharacter* OwnerChar = Cast<ACharacter>(GetOwner());
	if (!OwnerChar || !Montage) return false;
	UAnimInstance* AnimInst = OwnerChar->GetMesh() ? OwnerChar->GetMesh()->GetAnimInstance() : nullptr;
	if (!AnimInst) return false;
	AnimInst->Montage_Play(Montage);
	return true;
}

bool UCharacterCombatComponent::TryAttack()
{
	if (NameToCombo.Num() == 0)
	{
		if (!CanUse(LastAttackTime, AttackCooldown)) return false;
		UAnimMontage* M = PickMontage(AttackMontages, AttackPolicy, AttackSeqIndex);
		if (!PlayMontage(M)) return false;
		LastAttackTime = UGameplayStatics::GetTimeSeconds(this);
		return true;
	}
	return TryAttackBySet(CurrentAttackSet == NAME_None ? NameToCombo.CreateConstIterator().Key() : CurrentAttackSet);
}

bool UCharacterCombatComponent::TryAttackBySet(FName AttackSetName)
{
	return StartOrContinueCombo(AttackSetName);
}

bool UCharacterCombatComponent::TrySkill()
{
	if (NameToSkill.Num() == 0)
	{
		if (!CanUse(LastSkillTime, SkillCooldown)) return false;
		UAnimMontage* M = PickMontage(SkillMontages, SkillPolicy, SkillSeqIndex);
		if (!PlayMontage(M)) return false;
		LastSkillTime = UGameplayStatics::GetTimeSeconds(this);
		return true;
	}
	return TrySkillByName(NameToSkill.CreateConstIterator().Key());
}

bool UCharacterCombatComponent::TrySkillByName(FName SkillName)
{
	const FSkillSpec* Spec = NameToSkill.Find(SkillName);
	if (!Spec) return false;
	if (!CanUse(LastSkillTime, Spec->Cooldown)) return false;
	UAnimMontage* M = Spec->Montage.IsValid() ? Spec->Montage.Get() : Spec->Montage.LoadSynchronous();
	if (!PlayMontage(M)) return false;
	LastSkillTime = UGameplayStatics::GetTimeSeconds(this);
	DamageSkill = Spec->Damage;
	AttackMultiplierSkill = Spec->AttackMultiplier;
	HitRadiusSkill = Spec->HitRadius;
	HitRangeSkill = Spec->HitRange;
	HitSocketSkill = Spec->HitSocket;
	if (Spec->StartSound.IsValid() || !Spec->StartSound.IsNull())
	{
		USoundBase* S = Spec->StartSound.LoadSynchronous();
		if (S) UGameplayStatics::PlaySoundAtLocation(this, S, GetOwner()->GetActorLocation());
	}
	return true;
}

void UCharacterCombatComponent::BindAnimDelegates()
{
	ACharacter* OwnerChar = Cast<ACharacter>(GetOwner());
	if (!OwnerChar || !OwnerChar->GetMesh()) return;
	if (UAnimInstance* AnimInst = OwnerChar->GetMesh()->GetAnimInstance())
	{
		AnimInst->OnPlayMontageNotifyBegin.AddDynamic(this, &UCharacterCombatComponent::OnNotifyBegin);
		AnimInst->OnPlayMontageNotifyEnd.AddDynamic(this, &UCharacterCombatComponent::OnNotifyEnd);
	}
}

void UCharacterCombatComponent::OnNotifyBegin(FName NotifyName, const FBranchingPointNotifyPayload& Payload)
{
	if (NotifyName == AttackNotifyBegin) { bAttackWindow = true; }
	if (NotifyName == SkillNotifyBegin)  { bSkillWindow = true; }
	if (NotifyName == ComboWindowOpen)
	{
		bComboInputWindow = true;
		// â�� ���Ȱ� ť�� ������ �ٷ� ����
		if (QueuedComboCount > 0)
		{
			// �ٷ� �������� �ѱ� �� �ְ� ���
		}
	}
	if (NotifyName == ComboWindowClose)  { bComboInputWindow = false; }
}

void UCharacterCombatComponent::OnNotifyEnd(FName NotifyName, const FBranchingPointNotifyPayload& Payload)
{
	if (NotifyName == AttackNotifyEnd)
	{
		if (bAttackWindow) DoHitTrace(false);
		bAttackWindow = false;
		// �Է� ť�� ������ �Һ� �� ���� �������� ����
		if (QueuedComboCount > 0)
		{
			QueuedComboCount--;
			PlayCurrentComboStage();
			return;
		}
		// ť�� ������ ������ �������������� Ȯ���ϰ� ���� ó��
		if (CurrentAttackSet != NAME_None)
		{
			const FAttackComboSpec* Combo = NameToCombo.Find(CurrentAttackSet);
			if (!Combo || CurrentStageIndex >= Combo->Stages.Num())
			{
				EndCombo();
			}
		}
	}
	if (NotifyName == SkillNotifyEnd)
	{
		if (bSkillWindow) DoHitTrace(true);
		bSkillWindow = false;
	}
}

void UCharacterCombatComponent::ForceDoHitTrace(bool bSkill)
{
	DoHitTrace(bSkill);
}

void UCharacterCombatComponent::DoHitTrace(bool bSkill)
{
	ACharacter* OwnerChar = Cast<ACharacter>(GetOwner());
	if (!OwnerChar) return;

	const float Radius = bSkill ? HitRadiusSkill : HitRadiusAttack;
	const float Range  = bSkill ? HitRangeSkill : HitRangeAttack;
	float Damage = bSkill ? DamageSkill : DamageAttack;
	const FName Socket = bSkill ? HitSocketSkill : HitSocketAttack;

	// ������ �ɷ�ġ ����ġ ����: Damage += Attack * Multiplier
	if (UAbilityComponent* Abil = OwnerChar->FindComponentByClass<UAbilityComponent>())
	{
		const float AttackStat = Abil->GetAttribute(TEXT("Attack"), 0.f);
		const float Mult = bSkill ? AttackMultiplierSkill : AttackMultiplierAttack;
		Damage += AttackStat * Mult;
	}

	FVector Start;
	FRotator FacingRot = OwnerChar->GetActorRotation();
	if (OwnerChar->GetMesh() && Socket != NAME_None)
	{
		Start = OwnerChar->GetMesh()->GetSocketLocation(Socket);
	}
	else
	{
		Start = OwnerChar->GetActorLocation() + FVector(0,0,50);
	}
	const FVector End = Start + FacingRot.Vector() * Range;

	TArray<FHitResult> Hits;
	UKismetSystemLibrary::SphereTraceMulti(this,
		Start, End, Radius,
		UEngineTypes::ConvertToTraceType(ECC_Pawn), false,
		{}, EDrawDebugTrace::None, Hits, true);

	bool bAnyHit = false;
	for (const FHitResult& Hit : Hits)
	{
		AActor* Other = Hit.GetActor();
		if (Other && Other != OwnerChar)
		{
			UGameplayStatics::ApplyDamage(Other, Damage, OwnerChar->GetController(), OwnerChar, nullptr);
			bAnyHit = true;
		}
	}
	if (bAnyHit)
	{
		USoundBase* HitSnd = nullptr;
		if (!bSkill)
		{
			const FAttackComboSpec* Combo = NameToCombo.Find(CurrentAttackSet);
			int32 StageIndex = FMath::Max(0, CurrentStageIndex - 1);
			if (Combo && Combo->Stages.IsValidIndex(StageIndex))
			{
				const FAttackStageSpec& Stage = Combo->Stages[StageIndex];
				HitSnd = Stage.HitSound.IsValid() ? Stage.HitSound.Get() : Stage.HitSound.LoadSynchronous();
			}
		}
		if (HitSnd)
		{
			UGameplayStatics::PlaySoundAtLocation(this, HitSnd, GetOwner()->GetActorLocation());
		}
	}
}

bool UCharacterCombatComponent::StartOrContinueCombo(FName AttackSetName)
{
	// ������ ���� ����: ��Ÿ�ְ� �� �̻� ��� ���� �ƴѵ� CurrentAttackSet�� �����ִٸ� ���� ó��
	if (CurrentAttackSet != NAME_None)
	{
		ACharacter* OwnerChar = Cast<ACharacter>(GetOwner());
		UAnimInstance* AnimInst = (OwnerChar && OwnerChar->GetMesh()) ? OwnerChar->GetMesh()->GetAnimInstance() : nullptr;
		if (!AnimInst || !AnimInst->IsAnyMontagePlaying())
		{
			EndCombo();
		}
	}

	const FAttackComboSpec* Combo = NameToCombo.Find(AttackSetName);
	if (!Combo || Combo->Stages.Num() == 0) return false;

	const double Now = UGameplayStatics::GetTimeSeconds(this);

	const bool bSameSet = (CurrentAttackSet == AttackSetName);
	const bool bWithinReset = (ComboStartTime > 0.0) && (Now - ComboStartTime <= Combo->ComboResetTime);
	const bool bAfterCooldown = (LastComboEndTime < 0.0) || (Now - LastComboEndTime >= Combo->ComboCooldown);

	// �޺��� �̹� ���� ���� ��쿡�� ���� �������� ����
	if (CurrentAttackSet != NAME_None)
	{
		if (bSameSet)
		{
			QueuedComboCount = FMath::Clamp(QueuedComboCount + 1, 0, 4);
			return true; // ���� ��: �Է��� ť�� ���縸
		}
		return false; // �ٸ� ��Ʈ �Է��� ����
	}

	// �޺� ������ ����: ���� ����
	if (!bAfterCooldown) return false;
	CurrentAttackSet = AttackSetName;
	CurrentStageIndex = 0;
	ComboStartTime = Now;
	// ���� �Է� �ϳ� �Һ�(�ִٸ�)
	if (QueuedComboCount > 0) QueuedComboCount--;
	return PlayCurrentComboStage();
}

bool UCharacterCombatComponent::PlayCurrentComboStage()
{
	const FAttackComboSpec* Combo = NameToCombo.Find(CurrentAttackSet);
	if (!Combo) return false;
	if (!Combo->Stages.IsValidIndex(CurrentStageIndex))
	{
		EndCombo();
		return false;
	}
	const FAttackStageSpec& Stage = Combo->Stages[CurrentStageIndex];
	UAnimMontage* M = Stage.Montage.IsValid() ? Stage.Montage.Get() : Stage.Montage.LoadSynchronous();
	if (!PlayMontage(M))
	{
		EndCombo();
		return false;
	}
	DamageAttack = Stage.Damage;
	AttackMultiplierAttack = Stage.AttackMultiplier;
	HitRadiusAttack = Stage.HitRadius;
	HitRangeAttack = Stage.HitRange;
	HitSocketAttack = Stage.HitSocket;
	if (Stage.StartSound.IsValid() || !Stage.StartSound.IsNull())
	{
		USoundBase* S = Stage.StartSound.LoadSynchronous();
		if (S) UGameplayStatics::PlaySoundAtLocation(this, S, GetOwner()->GetActorLocation());
	}
	CurrentStageIndex++;
	return true;
}

void UCharacterCombatComponent::EndCombo()
{
	LastComboEndTime = UGameplayStatics::GetTimeSeconds(this);
	CurrentAttackSet = NAME_None;
	CurrentStageIndex = 0;
	ComboStartTime = -1.0;
	QueuedComboCount = 0;
	bComboInputWindow = false;
}
