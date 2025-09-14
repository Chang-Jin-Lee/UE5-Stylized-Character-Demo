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

	// 공격력 가중치(공격자 Attack 능력치에 대한 배수 가중치)
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

	// 콤보 입력 유효 시간(첫타 시작 후 이 시간이 지나면 콤보 리셋)
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float ComboResetTime = 1.0f;

	// 콤보 종료 후 다음 콤보까지 쿨다운
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float ComboCooldown = 0.6f;

	// 콤보 단계들(1타, 2타, ...)
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

	// 공격력 가중치(공격자 Attack 능력치에 대한 배수 가중치)
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
 * @brief : 캐릭터 선택을 데이터 주도형으로 정의하는 Primary Data Asset
 * @details :
 * 	- 런타임에 안전하게 로드 가능하며 에셋만으로 확장
 * 	- C++ 재컴파일 없이 신규 캐릭터 추가 가능
 */
UCLASS(BlueprintType)
class GAME_API UCharacterDefinition : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UCharacterDefinition();

	/*
	 * @brief : 가독성용 표시 식별자
	 * @details : 저장/네트워크 식별은 PrimaryAssetId 사용 권장
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character", meta = (Tooltip = "가독성을 위한 선택형 식별자 (저장은 PrimaryAssetId 사용)"))
	FName CharacterId;

	/*
	 * @brief : 선택 시 스폰할 폰 클래스
	 * @details : 비어 있으면 기본 AGameCharacter 스폰 후 아래 스펙을 동적으로 적용
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character|Spawn", meta = (Tooltip = "선택 시 스폰할 폰 클래스 (파생 Character 권장). 비우면 기본 Pawn에 스펙 적용"))
	TSoftClassPtr<APawn> PawnClass;

	/*
	 * @brief : 교체할 SkeletalMesh 및 AnimClass
	 * @details : 서로 다른 스켈레톤/리깅(MMD 등)도 Anim BP만 맞추면 런타임 교체 가능
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character|Visual", meta = (Tooltip = "런타임에 적용할 SkeletalMesh"))
	TSoftObjectPtr<USkeletalMesh> Mesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character|Visual", meta = (Tooltip = "런타임에 적용할 AnimInstance 클래스(AnimBlueprint)"))
	TSoftClassPtr<UAnimInstance> AnimClass;

	/*
	 * @brief : Mesh 컴포넌트의 상대 트랜스폼
	 * @details : 스케일/오프셋이 다른 모델(MMD 등)을 캡슐에 정렬하기 위해 사용
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character|Visual")
	FVector MeshRelativeLocation = FVector::ZeroVector;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character|Visual")
	FRotator MeshRelativeRotation = FRotator::ZeroRotator;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character|Visual")
	FVector MeshRelativeScale3D = FVector(1.f, 1.f, 1.f);

	/*
	 * @brief : 전투 스펙(다중 몽타주/콤보/쿨다운/데미지/트레이스)
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character|Combat", meta=(Tooltip="공격 콤보들(이름 → 단계들)"))
	TMap<FName, FAttackComboSpec> AttackCombos;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character|Combat", meta=(Tooltip="스킬들(이름 → 스펙)"))
	TMap<FName, FSkillSpec> Skills;

	/*
	 * @brief : (구버전) 다중 몽타주 배열 ? 필요 시 병행 사용
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character|Combat", meta=(Tooltip="(구버전) 공격 몽타주 목록"))
	TArray<TSoftObjectPtr<UAnimMontage>> AttackMontages;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character|Combat", meta=(Tooltip="(구버전) 스킬 몽타주 목록"))
	TArray<TSoftObjectPtr<UAnimMontage>> SkillMontages;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character|Combat", meta=(Tooltip="(구버전) 공격 몽타주 선택 정책"))
	EMontageSelectPolicy AttackSelectPolicy = EMontageSelectPolicy::Random;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character|Combat", meta=(Tooltip="(구버전) 스킬 몽타주 선택 정책"))
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
	 * @brief : 캡슐 사이즈(반지름/하프 높이)
	 * @details : 모델 신체 비례에 맞춘 충돌 정합성 확보
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character|Collision", meta=(ClampMin="0.0"))
	float CapsuleRadius = 42.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character|Collision", meta=(ClampMin="0.0"))
	float CapsuleHalfHeight = 96.f;

	/*
	 * @brief : 이동 관련 스펙
	 * @details : 캐릭터별 속도/점프/회전 정책을 손쉽게 변경
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character|Movement")
	float MaxWalkSpeed = 500.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character|Movement")
	float JumpZVelocity = 700.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character|Movement")
	bool bOrientRotationToMovement = true;

	/*
	 * @brief : 카메라 붐 길이 (선택)
	 * @details : 음수면 적용하지 않음
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character|Camera")
	float CameraBoomArmLength = -1.f;

	/*
	 * @brief : PrimaryAsset 타입 지정
	 * @details : 에디터/런타임에서 일관된 그룹 관리
	 */
	virtual FPrimaryAssetId GetPrimaryAssetId() const override;
};


