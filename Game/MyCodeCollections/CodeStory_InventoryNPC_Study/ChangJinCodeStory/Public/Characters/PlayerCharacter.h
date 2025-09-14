#pragma once

#include "CoreMinimal.h"
#include "Base_Character.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/Character.h"
#include "Widgets/NameInputWidget.h"
#include "PlayerCharacter.generated.h"

class ABase_Weapon;
class UInventoryUIWidget;
class UPlayerUIWidget;
class UNameTagWidget;
class USpringArmComponent;
class UInputAction;
class UCameraComponent;
class UInputMappingContext;

UCLASS()
class CHANGJINCODESTORY_API APlayerCharacter : public ABase_Character
{
	GENERATED_BODY()

public:
	APlayerCharacter();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;
	
	ABase_Character* NonPlayerCharacter;
	USkeletalMeshComponent* FindMeshByName(FName MeshName);
	
	void DrawWeaponFromInventory(int32 WeaponIndex);
	void DrawWeapon(FInventoryItem Itemdata);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weapons")
	ABase_Weapon* CurrentWeapon;
	
protected:
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	
private:
	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	
	UPROPERTY(VisibleAnywhere)
	UCameraComponent* CameraComponent;

	UPROPERTY(VisibleAnywhere)
	USpringArmComponent* CameraArm;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* MoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* LookAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* OpenInventoryAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;

	UPROPERTY(EditAnywhere, Category = "UI")
	UNameInputWidget* NameInputWidget;

	UPROPERTY(EditAnywhere, Category = "UI")
	UWidgetComponent* NameTagWidgetComponent;

	UFUNCTION()
	void CreateNameInputWidget();

	UFUNCTION()
	void SetNameTag();

	
};
