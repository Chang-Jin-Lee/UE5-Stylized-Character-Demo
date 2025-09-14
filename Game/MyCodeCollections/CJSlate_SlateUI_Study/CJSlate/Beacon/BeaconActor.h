// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BeaconActor.generated.h"

class AAnswerManager;
class UBoxComponent;

UCLASS()
class CJSLATE_API ABeaconActor : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ABeaconActor();

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

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Mesh")
	UBoxComponent* CollisionBox;

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_Move(FVector NewLocation);

	UFUNCTION(NetMulticast, Reliable, WithValidation)
	void Multicast_Move(FVector NewLocation);

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	// Beacon에 대한 정보와 매니저
	UPROPERTY(EditAnywhere)
	int32 BeaconID;

	UPROPERTY(EditAnywhere)
	FString BeaconName;

	UPROPERTY(EditAnywhere)
	FVector AddLocation = FVector(0, 0, 35);

	UPROPERTY(EditAnywhere)
	FVector StartingLocation;

	UPROPERTY(EditInstanceOnly)
	TSoftObjectPtr<AAnswerManager> BeaconManager;
};
