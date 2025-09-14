#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "UObject/PrimaryAssetId.h"
#include "CharacterDefinition.generated.h"

class USkeletalMesh;
class UAnimMontage;
class USoundBase;

UENUM(BlueprintType)
enum class EMontageSelectPolicy : uint8
{
	Random      UMETA(DisplayName="Random"),
	Sequence    UMETA(DisplayName="Sequence")
};

USTRUCT(BlueprintType)
struct FAttackStageSpec
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSoftObjectPtr<UAnimMontage> Montage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSoftObjectPtr<USoundBase> StartSound;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSoftObjectPtr<USoundBase> HitSound;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float Damage = 20.f;

	// ���ݷ� ����ġ(������ Attack �ɷ�ġ�� ���� ��� ����ġ)
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float AttackMultiplier = 0.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float HitRadius = 30.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float HitRange = 150.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FName HitSocket = NAME_None;
};

USTRUCT(BlueprintType)
struct FAttackComboSpec
{
	GENERATED_BODY()

	// �޺� �Է� ��ȿ �ð�(ùŸ ���� �� �� �ð��� ������ �޺� ����)
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float ComboResetTime = 1.0f;

	// �޺� ���� �� ���� �޺����� ��ٿ�
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float ComboCooldown = 0.6f;

	// �޺� �ܰ��(1Ÿ, 2Ÿ, ...)
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FAttackStageSpec> Stages;
};

USTRUCT(BlueprintType)
struct FSkillSpec
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSoftObjectPtr<UAnimMontage> Montage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSoftObjectPtr<USoundBase> StartSound;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSoftObjectPtr<USoundBase> HitSound;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float Cooldown = 3.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float Damage = 50.f;

	// ���ݷ� ����ġ(������ Attack �ɷ�ġ�� ���� ��� ����ġ)
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float AttackMultiplier = 0.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float HitRadius = 60.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float HitRange = 250.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FName HitSocket = NAME_None;
};

/*
 * @brief : ĳ���� ������ ������ �ֵ������� �����ϴ� Primary Data Asset
 * @details :
 * 	- ��Ÿ�ӿ� �����ϰ� �ε� �����ϸ� ���¸����� Ȯ��
 * 	- C++ �������� ���� �ű� ĳ���� �߰� ����
 */
UCLASS(BlueprintType)
class GAME_API UCharacterDefinition : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UCharacterDefinition();

	/*
	 * @brief : �������� ǥ�� �ĺ���
	 * @details : ����/��Ʈ��ũ �ĺ��� PrimaryAssetId ��� ����
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character", meta = (Tooltip = "�������� ���� ������ �ĺ��� (������ PrimaryAssetId ���)"))
	FName CharacterId;

	/*
	 * @brief : ���� �� ������ �� Ŭ����
	 * @details : ��� ������ �⺻ AGameCharacter ���� �� �Ʒ� ������ �������� ����
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character|Spawn", meta = (Tooltip = "���� �� ������ �� Ŭ���� (�Ļ� Character ����). ���� �⺻ Pawn�� ���� ����"))
	TSoftClassPtr<APawn> PawnClass;

	/*
	 * @brief : ��ü�� SkeletalMesh �� AnimClass
	 * @details : ���� �ٸ� ���̷���/����(MMD ��)�� Anim BP�� ���߸� ��Ÿ�� ��ü ����
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character|Visual", meta = (Tooltip = "��Ÿ�ӿ� ������ SkeletalMesh"))
	TSoftObjectPtr<USkeletalMesh> Mesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character|Visual", meta = (Tooltip = "��Ÿ�ӿ� ������ AnimInstance Ŭ����(AnimBlueprint)"))
	TSoftClassPtr<UAnimInstance> AnimClass;

	/*
	 * @brief : Mesh ������Ʈ�� ��� Ʈ������
	 * @details : ������/�������� �ٸ� ��(MMD ��)�� ĸ���� �����ϱ� ���� ���
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character|Visual")
	FVector MeshRelativeLocation = FVector::ZeroVector;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character|Visual")
	FRotator MeshRelativeRotation = FRotator::ZeroRotator;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character|Visual")
	FVector MeshRelativeScale3D = FVector(1.f, 1.f, 1.f);

	/*
	 * @brief : ���� ����(���� ��Ÿ��/�޺�/��ٿ�/������/Ʈ���̽�)
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character|Combat", meta=(Tooltip="���� �޺���(�̸� �� �ܰ��)"))
	TMap<FName, FAttackComboSpec> AttackCombos;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character|Combat", meta=(Tooltip="��ų��(�̸� �� ����)"))
	TMap<FName, FSkillSpec> Skills;

	/*
	 * @brief : (������) ���� ��Ÿ�� �迭 ? �ʿ� �� ���� ���
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character|Combat", meta=(Tooltip="(������) ���� ��Ÿ�� ���"))
	TArray<TSoftObjectPtr<UAnimMontage>> AttackMontages;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character|Combat", meta=(Tooltip="(������) ��ų ��Ÿ�� ���"))
	TArray<TSoftObjectPtr<UAnimMontage>> SkillMontages;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character|Combat", meta=(Tooltip="(������) ���� ��Ÿ�� ���� ��å"))
	EMontageSelectPolicy AttackSelectPolicy = EMontageSelectPolicy::Random;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character|Combat", meta=(Tooltip="(������) ��ų ��Ÿ�� ���� ��å"))
	EMontageSelectPolicy SkillSelectPolicy = EMontageSelectPolicy::Random;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character|Combat")
	float AttackCooldown = 0.6f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character|Combat")
	float SkillCooldown = 3.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character|Combat")
	float DamageAttack = 20.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character|Combat")
	float DamageSkill = 50.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character|Combat")
	float HitRadiusAttack = 30.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character|Combat")
	float HitRangeAttack = 150.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character|Combat")
	FName HitSocketAttack = NAME_None;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character|Combat")
	float HitRadiusSkill = 60.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character|Combat")
	float HitRangeSkill = 250.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character|Combat")
	FName HitSocketSkill = NAME_None;

	/*
	 * @brief : ĸ�� ������(������/���� ����)
	 * @details : �� ��ü ��ʿ� ���� �浹 ���ռ� Ȯ��
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character|Collision", meta=(ClampMin="0.0"))
	float CapsuleRadius = 42.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character|Collision", meta=(ClampMin="0.0"))
	float CapsuleHalfHeight = 96.f;

	/*
	 * @brief : �̵� ���� ����
	 * @details : ĳ���ͺ� �ӵ�/����/ȸ�� ��å�� �ս��� ����
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character|Movement")
	float MaxWalkSpeed = 500.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character|Movement")
	float JumpZVelocity = 700.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character|Movement")
	bool bOrientRotationToMovement = true;

	/*
	 * @brief : ī�޶� �� ���� (����)
	 * @details : ������ �������� ����
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character|Camera")
	float CameraBoomArmLength = -1.f;

	/*
	 * @brief : PrimaryAsset Ÿ�� ����
	 * @details : ������/��Ÿ�ӿ��� �ϰ��� �׷� ����
	 */
	virtual FPrimaryAssetId GetPrimaryAssetId() const override;
};


