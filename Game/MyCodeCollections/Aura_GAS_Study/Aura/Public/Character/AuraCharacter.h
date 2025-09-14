// Copyright ChangJin

#pragma once

#include "CoreMinimal.h"
#include "AuraCharacterBase.h"
#include "AuraCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;

UCLASS()
class AURA_API AAuraCharacter : public AAuraCharacterBase
{
	GENERATED_BODY()

public:
	AAuraCharacter();
	//~ Begin APawn Interface.
	virtual void PossessedBy(AController* NewController) override;
	
	/** PlayerState Replication Notification Callback */
	virtual void OnRep_PlayerState() override;

	/* Combat Interface */
	virtual int32 GetPlayerLevel() override;

	UPROPERTY(EditAnywhere,Category="Camera")
	UCameraComponent* MainCharacterCamera;

	UPROPERTY(EditAnywhere,Category="Camera")
	USpringArmComponent* MainCharacterCameraArm;
private:
	virtual void InitAbilityActorInfo() override;
};
