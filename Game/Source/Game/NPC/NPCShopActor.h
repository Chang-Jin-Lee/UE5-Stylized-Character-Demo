#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NPCShopActor.generated.h"

class USphereComponent;

/*
 * @brief : ���� NPC (���� �� ���� �� ��ȣ�ۿ� ���)
 */
UCLASS()
class GAME_API ANPCShopActor : public AActor
{
	GENERATED_BODY()
public:
	ANPCShopActor();

protected:
	UPROPERTY(VisibleAnywhere, Category="NPC")
	USphereComponent* DetectionSphere;

	UPROPERTY(EditAnywhere, Category="NPC")
	float DetectionRadius = 300.f;
};


