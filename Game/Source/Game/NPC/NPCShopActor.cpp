#include "Game/NPC/NPCShopActor.h"
#include "Components/SphereComponent.h"

ANPCShopActor::ANPCShopActor()
{
	PrimaryActorTick.bCanEverTick = false;
	DetectionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("DetectionSphere"));
	RootComponent = DetectionSphere;
	DetectionSphere->InitSphereRadius(DetectionRadius);
	DetectionSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	DetectionSphere->SetCollisionResponseToAllChannels(ECR_Ignore);
	DetectionSphere->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
}


