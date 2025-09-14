// Fill out your copyright notice in the Description page of Project Settings.

#include "Lena/Actor/InteractableThings/Portal/PortalActor.h"
#include "Lena/Controller/ShooterPlayerController.h"
#include "TimerManager.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "Lena/GameInstance/LenaGameInstance.h"
#include "Lena/GameInstanceSubSystem/LenaRoom01Manager.h"
#include "Lena/HUD/Lena_Room01HUD.h"
#include "Lena/UI/InteractWidget.h"
#include "Lena/UI/Lena_InGameWIdget.h"


// Sets default values
APortalActor::APortalActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	HitRegieon = CreateDefaultSubobject<UBoxComponent>(TEXT("HitRegieon"));
	if(HitRegieon)
	{
		HitRegieon->SetupAttachment(GetRootComponent());
	}
	HitRegieon->OnComponentBeginOverlap.AddDynamic(this, &APortalActor::OnPortalOverlapBegin);
}

// Called when the game starts or when spawned
void APortalActor::BeginPlay()
{
	Super::BeginPlay();
	SetInstructionWidgetText(FText::FromString("Press E To Restart!"), FLinearColor::Blue);
}

// Called every frame
void APortalActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	UpdateWidget();
}

void APortalActor::Restart()
{
	UGameplayStatics::OpenLevel(this, FName(*GetWorld()->GetName()), false);
}

void APortalActor::MoveCharacterToLocation()
{
	ACharacter* PlayerCharacter = UGameplayStatics::GetPlayerCharacter(this, 0);
	if(PlayerCharacter)
	{
		PlayerCharacter->SetActorLocation(TargetLocation);
	}
}

void APortalActor::OnPortalOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ShowWidget();
	GetWorld()->GetTimerManager().SetTimer(RestartTimer, this, &APortalActor::OnRestart,  5.0f, false);
}

void APortalActor::OnRestart()
{
	hideWidget();
	if(PortalType == EPortalType::IPt_OpenLevel)
		UGameplayStatics::OpenLevel(this, FName(*LevelName), false);
	else if ( PortalType == EPortalType::IPt_Move)
		MoveCharacterToLocation();
}

void APortalActor::UpdateWidget()
{
	if(GetWorld()->GetTimerManager().IsTimerActive(RestartTimer))
	{
		int32 remianingtime = FMath::CeilToInt(GetWorld()->GetTimerManager().GetTimerRemaining(RestartTimer));

		if(g_LenaRoom01ManagerIsValid)
		{
			if(g_LenaRoom01Manager->GetInGameWidget())
				g_LenaRoom01Manager->GetInGameWidget()->SetDescriptionText(FString::FromInt(remianingtime));
		}
	}
}

void APortalActor::ShowWidget()
{
	if(g_LenaRoom01ManagerIsValid)
	{
		if(g_LenaRoom01Manager->GetInGameWidget())
		{
			g_LenaRoom01Manager->GetInGameWidget()->VisibleDescriptionText();
		}
	}
}

void APortalActor::hideWidget()
{
	if(g_LenaRoom01ManagerIsValid)
	{
		if(g_LenaRoom01Manager->GetInGameWidget())
		{
			g_LenaRoom01Manager->GetInGameWidget()->HiddenDescriptionText();
		}
	}
}