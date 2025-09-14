// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CubeUpdateActor.generated.h"

UCLASS()
class CJSLATE_API ACubeUpdateActor : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ACubeUpdateActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_MoveCube(FVector NewLocation);

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_MoveCube(FVector NewLocation);

	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Mesh")
	USceneComponent* SceneComponent;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UStaticMeshComponent* CubeMesh;
};
