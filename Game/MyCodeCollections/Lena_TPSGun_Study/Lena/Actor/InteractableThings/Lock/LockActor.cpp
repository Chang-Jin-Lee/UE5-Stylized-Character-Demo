
#include "LockActor.h"
#include "Kismet/GameplayStatics.h"

ALockActor::ALockActor()
{
	
}

void ALockActor::BeginPlay()
{
	Super::BeginPlay();
}

void ALockActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

bool ALockActor::CheckPassword(const FString& InputPassword)
{
	if(InputPassword==Password)
	{
		if(IsZoomedIn)
			ZoomOutCamera();
		SetActorEnableCollision(false);
		return true;
	}
	return false;
}

void ALockActor::Unlock(AActor* ActorToUnlock)
{
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), RightAnswerSound, GetActorLocation());
}
