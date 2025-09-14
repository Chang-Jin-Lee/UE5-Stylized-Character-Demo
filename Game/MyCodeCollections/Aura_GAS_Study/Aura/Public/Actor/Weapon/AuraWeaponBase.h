// Copyright ChangJin

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interaction/EnemyInterface.h"
#include "Interaction/WeaponInterface.h"
#include "AuraWeaponBase.generated.h"

UCLASS()
class AURA_API AAuraWeaponBase : public AActor, public IEnemyInterface
{
	GENERATED_BODY()
	
public:	
	AAuraWeaponBase();
	virtual void HighlightActor() override;
	virtual void UnHighlightActor() override;
	
	UPROPERTY(BlueprintReadOnly)
	bool bHightlighted = false;

	UPROPERTY(EditAnywhere)
	USkeletalMeshComponent* WeaponSkeletalMeshComponent;

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

};
