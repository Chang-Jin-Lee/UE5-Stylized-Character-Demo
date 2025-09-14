// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Base_Character.h"
#include "Components/WidgetComponent.h"
#include "Lena/UI/Lena_InGameWIdget.h"
#include "AI_Character.generated.h"

class ULena_InGameAIHealthBarWidget;

UCLASS()
class LENA_API AAI_Character : public ABase_Character
{
	GENERATED_BODY()

public:
	AAI_Character();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintImplementableEvent, Category="AIGun")
	void AIAimTarget();

	UFUNCTION(BlueprintImplementableEvent, Category="AIGun")
	void AILooseAimTarget();

	UPROPERTY(EditAnywhere, Category="UI")
	TObjectPtr<UWidgetComponent> HealthBarWidget;

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

private:
	UPROPERTY()
	TObjectPtr<AGun> Gun;
	
	UPROPERTY(EditDefaultsOnly, Category="Weapon")
	TArray<TSubclassOf<AGun>> GunClassArray;
	
	UPROPERTY(BlueprintReadWrite, Category="Weapon", meta=(AllowPrivateAccess="true"))
	TArray<TObjectPtr<AGun>> GunArray;

	UPROPERTY(BlueprintReadWrite, Category="Weapon", meta=(AllowPrivateAccess="true"))
	int WeaponActiveIndex = 0;

	UPROPERTY()
	TSoftObjectPtr<ULena_InGameWIdget> InGameWidget;
	UPROPERTY()
	bool IsDeadCounted = false;
};
