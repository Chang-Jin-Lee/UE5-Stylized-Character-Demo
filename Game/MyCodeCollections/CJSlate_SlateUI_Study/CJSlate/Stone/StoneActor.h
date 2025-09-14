// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CJSlate/Interface/MoveInterface.h"
#include "GameFramework/Actor.h"
#include "StoneActor.generated.h"

class UWidgetComponent;

UCLASS()
class CJSLATE_API AStoneActor : public AActor, public IMoveInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AStoneActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Mesh")
	USceneComponent* SceneComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Mesh")
	UStaticMeshComponent* MeshComp;

	UPROPERTY(EditDefaultsOnly)
	UWidgetComponent* WidgetComponent;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Move")
	bool IsMoved;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Move")
	int32 StoneID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Move")
	FMoveData MoveData;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Manager")
	TSoftObjectPtr<class AAnswerManager> AnswerManager;

	UFUNCTION()
	virtual void HandleEventProgress(float Value) override;
	virtual const FMoveData& GetMoveData() const override { return MoveData; }

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_Move();
	
	UFUNCTION(NetMulticast, Reliable, WithValidation)
	void MultiCast_Move();
	
	void Move();
};
