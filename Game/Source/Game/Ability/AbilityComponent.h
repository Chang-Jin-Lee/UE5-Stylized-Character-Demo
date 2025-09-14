#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AbilityComponent.generated.h"

class UAbilitySettings;
class AActor;

/*
 * @brief : �ɷ�ġ ����/���� ������Ʈ (���� �Ӽ�, ���� Ű �߰� ����)
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class GAME_API UAbilityComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UAbilityComponent();

	// �ʱ�ȭ: �������� �⺻�� �ε�
	virtual void BeginPlay() override;

	// �б�/����
	UFUNCTION(BlueprintCallable, Category="Ability")
	float GetAttribute(FName Name, float DefaultIfMissing = 0.f) const;

	UFUNCTION(BlueprintCallable, Category="Ability")
	void SetAttribute(FName Name, float Value);

	UFUNCTION(BlueprintCallable, Category="Ability")
	void AddAttribute(FName Name, float Delta);

	// ü�� ����
	UFUNCTION(BlueprintCallable, Category="Ability")
	float GetHealth() const { return GetAttribute(TEXT("Health")); }

	UFUNCTION(BlueprintCallable, Category="Ability")
	void ApplyDamageAmount(float Amount);

	// ������ ���� ��
	UFUNCTION()
	void OnAnyDamageTaken(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);

private:
	UPROPERTY(VisibleAnywhere, Category="Ability")
	TMap<FName, float> Attributes;

public:
	// ���� �� ��ȸ��(C++)
	const TMap<FName, float>& GetAttributes() const { return Attributes; }
};


