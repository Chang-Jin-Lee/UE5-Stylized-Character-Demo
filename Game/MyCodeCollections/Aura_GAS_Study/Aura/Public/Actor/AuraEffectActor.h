// Copyright ChangJin

#pragma once

#include "CoreMinimal.h"
#include "ActiveGameplayEffectHandle.h"
#include "GameFramework/Actor.h"
#include "AuraEffectActor.generated.h"

class UAbilitySystemComponent;
class UGameplayEffect;
class USphereComponent;

UENUM(BlueprintType)
enum class EEfectApplicationPolicy : uint8
{
	ApplyOnOverlap,
	ApplyOnEndOverlap,
	DoNotApply
};

UENUM(BlueprintType)
enum class EEfectRemovalPolicy : uint8
{
	RemoveOnEndOverlap,
	DoNotApply
};

USTRUCT(BlueprintType)
struct FGameplayEffectStruct
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Applied Effects")
	TSubclassOf<UGameplayEffect> GameplayEffectClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Applied Effects")
	EEfectApplicationPolicy ApplicationPolicy = EEfectApplicationPolicy::DoNotApply;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Applied Effects")
	EEfectRemovalPolicy EffectRemovalPolicy = EEfectRemovalPolicy::RemoveOnEndOverlap;
	
};

UCLASS()
class AURA_API AAuraEffectActor : public AActor
{
	GENERATED_BODY()
	
public:	
	AAuraEffectActor();
protected:
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	void ApplyEffectToTarget(AActor* TargetActor, const FGameplayEffectStruct& GampeplayEffectStruct);

	UFUNCTION(BlueprintCallable)
	void OnOverlap(AActor* TargetActor);

	UFUNCTION(BlueprintCallable)
	void OnEndOverlap(AActor* TargetActor);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Applied Effects")
	bool bDestroyOnEffectRemoval = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Applied Effects")
	TArray<FGameplayEffectStruct> InstantGameplayEffectArray;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Applied Effects")
	TArray<FGameplayEffectStruct> DurationGameplayEffectArray;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Applied Effects")
	TArray<FGameplayEffectStruct> InfiniteGameplayEffectArray;

	TMap<FActiveGameplayEffectHandle, UAbilitySystemComponent*> ActiveEffectHandles;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Applied Effects")
	float ActorLevel = 1.0f;
};
