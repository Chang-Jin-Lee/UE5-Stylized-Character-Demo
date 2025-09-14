// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CJSlate/Interface/MoveInterface.h"
#include "Components/TimelineComponent.h"
#include "GameFramework/Actor.h"
#include "DoorActor.generated.h"

class UTimelineComponent;

UCLASS()
class CJSLATE_API ADoorActor : public AActor, public IMoveInterface
{
	GENERATED_BODY()

public:
	ADoorActor();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Mesh")
	USceneComponent* SceneComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Mesh")
	UStaticMeshComponent* MeshComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Move")
	FMoveData MoveData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool IsOpened;

	UFUNCTION()
	virtual void HandleEventProgress(float Value) override;

	virtual const FMoveData& GetMoveData() const override { return MoveData; }

	UFUNCTION()
	void OpenDoor();
};