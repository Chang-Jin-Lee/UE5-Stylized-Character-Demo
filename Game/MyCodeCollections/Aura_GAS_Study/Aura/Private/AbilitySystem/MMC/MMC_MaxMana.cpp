// Copyright ChangJin


#include "AbilitySystem/MMC/MMC_MaxMana.h"

#include "AbilitySystem/AuraAttributeSet.h"
#include "Interaction/CombatInterface.h"

UMMC_MaxMana::UMMC_MaxMana()
{
	InteligenceDefinition.AttributeToCapture = UAuraAttributeSet::GetInteligenceAttribute();
	InteligenceDefinition.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
	InteligenceDefinition.bSnapshot = false;
	RelevantAttributesToCapture.Add(InteligenceDefinition);
}

float UMMC_MaxMana::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	FAggregatorEvaluateParameters EvaluateParams;
	EvaluateParams.SourceTags = SourceTags;
	EvaluateParams.TargetTags = TargetTags;

	float Inteligence = 0.0f;
	GetCapturedAttributeMagnitude(InteligenceDefinition, Spec, EvaluateParams, Inteligence);
	Inteligence = FMath::Max<float>(Inteligence, 0.0f);

	ICombatInterface* CombatInterface = Cast<ICombatInterface>(Spec.GetContext().GetSourceObject());
	const int32 PlayerLevel = CombatInterface->GetPlayerLevel();

	return 50 + 2.5f * Inteligence + 10.f * PlayerLevel;
}
