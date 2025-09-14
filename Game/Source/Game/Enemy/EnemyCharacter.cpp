#include "Game/Enemy/EnemyCharacter.h"
#include "Game/Enemy/EnemyDefinition.h"
#include "Game/Combat/CharacterCombatComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Game/Ability/AbilityComponent.h"
#include "UObject/ConstructorHelpers.h"

AEnemyCharacter::AEnemyCharacter()
{
	PrimaryActorTick.bCanEverTick = false;
	CombatComponent = CreateDefaultSubobject<UCharacterCombatComponent>(TEXT("CombatComponent"));
	AbilityComponent = CreateDefaultSubobject<UAbilityComponent>(TEXT("AbilityComponent"));

	// Quinn 기본 스켈레탈메시/애님BP 폴백 설정 (경로는 프로젝트 구성에 맞게 존재해야 함)
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> QuinnMesh(TEXT("SkeletalMesh'/Game/Characters/Mannequins/Meshes/SKM_Quinn.SKM_Quinn'"));
	if (QuinnMesh.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(QuinnMesh.Object);
		GetMesh()->SetRelativeLocation(FVector(0.f, 0.f, -90.f));
		GetMesh()->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));
	}

	static ConstructorHelpers::FClassFinder<UAnimInstance> QuinnABP(TEXT("AnimBlueprint'/Game/Characters/Mannequins/Animations/ABP_Quinn.ABP_Quinn_C'"));
	if (QuinnABP.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(QuinnABP.Class);
	}

	// 기본 이동 파라미터
	GetCharacterMovement()->MaxWalkSpeed = 400.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2048.f;
}

void AEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();
	if (EnemySpec.IsValid())
	{
		const UEnemyDefinition* Spec = EnemySpec.LoadSynchronous();
		if (Spec)
		{
			GetCharacterMovement()->MaxWalkSpeed = Spec->MaxWalkSpeed;
		}
	}
}


