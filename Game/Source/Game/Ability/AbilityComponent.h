#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AbilityComponent.generated.h"

class UAbilitySettings;
class AActor;

/*
 * @brief : 능력치 저장/갱신 컴포넌트 (동적 속성, 임의 키 추가 가능)
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class GAME_API UAbilityComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UAbilityComponent();

	// 초기화: 설정에서 기본값 로드
	virtual void BeginPlay() override;

	// 읽기/쓰기
	UFUNCTION(BlueprintCallable, Category="Ability")
	float GetAttribute(FName Name, float DefaultIfMissing = 0.f) const;

	UFUNCTION(BlueprintCallable, Category="Ability")
	void SetAttribute(FName Name, float Value);

	UFUNCTION(BlueprintCallable, Category="Ability")
	void AddAttribute(FName Name, float Delta);

	// 체력 헬퍼
	UFUNCTION(BlueprintCallable, Category="Ability")
	float GetHealth() const { return GetAttribute(TEXT("Health")); }

	UFUNCTION(BlueprintCallable, Category="Ability")
	void ApplyDamageAmount(float Amount);

	// 데미지 수신 훅
	UFUNCTION()
	void OnAnyDamageTaken(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);

private:
	UPROPERTY(VisibleAnywhere, Category="Ability")
	TMap<FName, float> Attributes;

public:
	// 내부 맵 조회용(C++)
	const TMap<FName, float>& GetAttributes() const { return Attributes; }
};


