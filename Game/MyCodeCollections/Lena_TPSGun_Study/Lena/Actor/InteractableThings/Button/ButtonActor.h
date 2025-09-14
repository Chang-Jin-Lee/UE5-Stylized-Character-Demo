// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Lena/Actor/InteractableThings/InteractableActor.h"
#include "Lena/Actor/InteractableThings/Door/DoorActor.h"
#include "Lena/Actor/Items/Base_Item.h"
#include "ButtonActor.generated.h"

UCLASS()
class LENA_API AButtonActor : public AInteractableActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AButtonActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, Category="Touch")
	TArray<FString> RequiredItem;

	UPROPERTY(EditAnywhere, Category="Touch")
	TObjectPtr<ADoorActor> DoorActor;
	
	UPROPERTY(VisibleAnywhere, Category = "Materials")
	TObjectPtr<UMaterialInstanceDynamic> BaseMaterial;

	UPROPERTY(VisibleAnywhere, Category = "Materials")
	TObjectPtr<UMaterialInstanceDynamic> BrightnessMaterial;
	

	bool CheckConditions();

	UFUNCTION(BlueprintCallable)
	void OpenDoor();
	UFUNCTION()
	void OnEmissiveChange();

private:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> ButtonMesh;
	
	FTimerHandle WrongAnswerDelayHandle;
	FTimerHandle EmissiveHandle;
};
