#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameMathCubeActor.generated.h"

class UBoxComponent;

UCLASS()
class GAMEMATH_API AGameMathCubeActor : public AActor
{
	GENERATED_BODY()
	
public:	
	AGameMathCubeActor();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	void Move(float DeltaTime);
	void BoundCheck();
	FVector ReflectVector(FVector dir, FVector inNormal);

	UPROPERTY(EditAnywhere)
	float moveSpeed = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector moveDir;

	UPROPERTY(EditAnywhere)
	UBoxComponent* Boxcomp;

	UFUNCTION()
	void OnBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);
};
