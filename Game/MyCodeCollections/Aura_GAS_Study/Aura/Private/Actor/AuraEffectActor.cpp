// Copyright ChangJin


#include "Actor/AuraEffectActor.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"

AAuraEffectActor::AAuraEffectActor()
{
	PrimaryActorTick.bCanEverTick = false;

	SetRootComponent(CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot")));
}

void AAuraEffectActor::BeginPlay()
{
	Super::BeginPlay();
}

void AAuraEffectActor::ApplyEffectToTarget(AActor* TargetActor, const FGameplayEffectStruct& GampeplayEffectStruct)
{
	UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
	if(TargetASC == nullptr) return;

	TSubclassOf<UGameplayEffect> GampeplayEffectClass = GampeplayEffectStruct.GameplayEffectClass;
	check(GampeplayEffectClass);
	FGameplayEffectContextHandle EffectContextHandle = TargetASC->MakeEffectContext();
	EffectContextHandle.AddSourceObject(this);
	const FGameplayEffectSpecHandle EffectSpecHandle = TargetASC->MakeOutgoingSpec(GampeplayEffectClass, ActorLevel, EffectContextHandle);
	const FActiveGameplayEffectHandle ActiveEffectHandle = TargetASC->ApplyGameplayEffectSpecToSelf(*EffectSpecHandle.Data.Get());

	const bool bIsInfinite = EffectSpecHandle.Data.Get()->Def.Get()->DurationPolicy == EGameplayEffectDurationType::Infinite;

	if(bIsInfinite && GampeplayEffectStruct.EffectRemovalPolicy == EEfectRemovalPolicy::RemoveOnEndOverlap)
	{
		ActiveEffectHandles.Add(ActiveEffectHandle, TargetASC);
	}
}

void AAuraEffectActor::OnOverlap(AActor* TargetActor)
{
	for(FGameplayEffectStruct& GameplayEffect : InstantGameplayEffectArray)
	{
		if(GameplayEffect.ApplicationPolicy == EEfectApplicationPolicy::ApplyOnOverlap)
		{
			ApplyEffectToTarget(TargetActor, GameplayEffect);
		}
	}
	for(FGameplayEffectStruct& GameplayEffect : DurationGameplayEffectArray)
	{
		if(GameplayEffect.ApplicationPolicy == EEfectApplicationPolicy::ApplyOnOverlap)
		{
			ApplyEffectToTarget(TargetActor, GameplayEffect);
		}
	}
	for(FGameplayEffectStruct& GameplayEffect : InfiniteGameplayEffectArray)
	{
		if(GameplayEffect.ApplicationPolicy == EEfectApplicationPolicy::ApplyOnOverlap)
		{
			ApplyEffectToTarget(TargetActor, GameplayEffect);
		}
	}
}

void AAuraEffectActor::OnEndOverlap(AActor* TargetActor)
{
	for(FGameplayEffectStruct& GameplayEffect : InstantGameplayEffectArray)
	{
		if(GameplayEffect.ApplicationPolicy == EEfectApplicationPolicy::ApplyOnEndOverlap)
		{
			ApplyEffectToTarget(TargetActor, GameplayEffect);
		}
	}
	for(FGameplayEffectStruct& GameplayEffect : DurationGameplayEffectArray)
	{
		if(GameplayEffect.ApplicationPolicy == EEfectApplicationPolicy::ApplyOnEndOverlap)
		{
			ApplyEffectToTarget(TargetActor, GameplayEffect);
		}
	}

	UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
	if(!IsValid(TargetASC)) return;
	
	TArray<FActiveGameplayEffectHandle> HandlesToRemove;
	for(FGameplayEffectStruct& GameplayEffect : InfiniteGameplayEffectArray)
	{
		if(GameplayEffect.ApplicationPolicy == EEfectApplicationPolicy::ApplyOnEndOverlap)
		{
			ApplyEffectToTarget(TargetActor, GameplayEffect);
		}

		if(GameplayEffect.EffectRemovalPolicy == EEfectRemovalPolicy::RemoveOnEndOverlap)
		{
			for(TTuple<FActiveGameplayEffectHandle, UAbilitySystemComponent*> HandlePair : ActiveEffectHandles)
			{
				if(TargetASC == HandlePair.Value)
				{
					TargetASC->RemoveActiveGameplayEffect(HandlePair.Key, 1);
					HandlesToRemove.Add(HandlePair.Key);
				}
			}
		}
	}
	for(FActiveGameplayEffectHandle& Handle : HandlesToRemove)
	{
		ActiveEffectHandles.FindAndRemoveChecked(Handle);
	}
}

