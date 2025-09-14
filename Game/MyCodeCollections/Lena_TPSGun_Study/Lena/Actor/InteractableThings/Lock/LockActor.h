#pragma once

#include "CoreMinimal.h"
#include "Lena/Actor/InteractableThings/InteractableActor.h"
#include "GameFramework/Actor.h"
#include "Lena/Actor/InteractableThings/Door/DoorActor.h"
#include "LockActor.generated.h"

UCLASS()
class LENA_API ALockActor : public AInteractableActor
{
	GENERATED_BODY()

public:
	ALockActor();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Lock")
	FString Password;

	UFUNCTION(BlueprintCallable, Category="Lock")
	virtual bool CheckPassword(const FString& InputPassword);

	UPROPERTY(BlueprintReadWrite, Category="Lock")
	TObjectPtr<ADoorActor> TargetDoor;

	UFUNCTION(BlueprintCallable, Category="Lock")
	virtual void Unlock(AActor* ActorToUnlock);

protected:
	virtual void BeginPlay() override;

	void TriggerUnlockEvent(AActor* ActorToUnlock);

public:
	virtual void Tick(float DeltaTime) override;
	
};