#pragma once

/*
 * @briefs 벡터장에서 스폰될 액터입니다. 이 액터는 각각 다른 특성을 가질 수 있습니다.
 */

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "VectorFieldActor.generated.h"

UCLASS()
class GAMEMATH_API AVectorFieldActor : public AActor
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability")
	float repelDistance = 200.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability")
	float attractDistance = 400.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability")
	float repelForce = 2000.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability")
	float attractForce = 4000.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
	UStaticMeshComponent* repelMesh;
	
public:	
	AVectorFieldActor();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

};
