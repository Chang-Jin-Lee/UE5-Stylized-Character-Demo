#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/DataTable.h"
#include "ManagerRegistry/Data/LenaRoom01JsonData.h"
#include "ItemSpawnManager.generated.h"

class ABase_Item;
struct FItemSpawnData;
// 아이템 스폰 매니저 클래스
UCLASS()
class LENA_API AItemSpawnManager : public AActor
{
	GENERATED_BODY()

public:
	// 생성자: 기본 값 설정
	AItemSpawnManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	

private:
	// 템플릿을 사용하여 아이템 속성을 설정하는 함수
	void SetItemProperties(TObjectPtr<ABase_Item> SpawnedItem, FLenaRoom01ItemSpawnRow Row);
	
	// DataTable에서 아이템을 스폰하는 함수
	void SpawnItemsFromTable();
	void SpawnItem(FLenaRoom01ItemSpawnRow);
};
