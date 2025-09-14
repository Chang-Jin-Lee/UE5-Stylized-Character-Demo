// Fill out your copyright notice in the Description page of Project Settings.


#include "ShopPanelSlot.h"

#include "Blueprint/DragDropOperation.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Blueprint/WidgetTree.h"
#include "Components/Border.h"
#include "Components/BorderSlot.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/HorizontalBox.h"
#include "Components/HorizontalBoxSlot.h"
#include "Components/Image.h"
#include "Components/SizeBox.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"
#include "Components/VerticalBoxSlot.h"
#include "Kismet/GameplayStatics.h"
#include "PickUpDefense/Managers/SpawnManager.h"
#include "PickUpDefense/Units/PlayerUnit/BaseUnit.h"

void UShopPanelSlot::NativeOnDragCancelled(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	FVector2D MousePosition = UWidgetLayoutLibrary::GetMousePositionOnViewport(GetWorld());

	// Get the current screen resolution
	FVector2D ViewportSize;
	GEngine->GameViewport->GetViewportSize(ViewportSize);

	// Define the target resolution (1920x1080)
	FVector2D TargetResolution(1920.0f, 1080.0f);

	// Calculate the scale factor
	FVector2D ScaleFactor = ViewportSize / TargetResolution;

	// Scale the mouse position to match the actual screen resolution
	FVector2D ScaledMousePosition = MousePosition * ScaleFactor;
	
	 // Get the player controller
    APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
    if (!PlayerController) return;

    // Convert Mouse Position to a 3D World Position and Direction
    FVector WorldLocation, WorldDirection;
    PlayerController->DeprojectScreenPositionToWorld(ScaledMousePosition.X, ScaledMousePosition.Y, WorldLocation, WorldDirection);

    // Define ray trace parameters
    FVector Start = WorldLocation;
    FVector End = WorldLocation + (WorldDirection * 10000.0f); // Extend the ray to 10000 units

    FHitResult HitResult;
    FCollisionQueryParams Params;
    Params.AddIgnoredActor(PlayerController->GetPawn()); // Ignore the player's pawn

    // Perform raycast
    if (ensure(GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility, Params)))
    {
        // If the raycast hits something, log the location
        FVector SpawnLocation = HitResult.ImpactPoint;
        FRotator SpawnRotation = FRotator::ZeroRotator;

    	// Add an offset to the spawn location based on the surface normal
    	float OffsetDistance = 10.0f; // Offset the sphere by 10 units above the surface
    	SpawnLocation += HitResult.Normal * OffsetDistance;

    	// Ensure the world and class are valid before spawning
    	if (ensure(GetWorld()) && ensure(ABaseUnit::StaticClass()))
    	{
    		// Create a new Actor to hold the StaticMeshComponent
    		//  		ABaseUnit* SpawnedABaseUnitActor = GetWorld()->SpawnActor<ABaseUnit>(ABaseUnit::StaticClass(), SpawnLocation, SpawnRotation);
    		//  		UStaticMesh* LoadedMesh = Cast<UStaticMesh>(StaticLoadObject(UStaticMesh::StaticClass(), nullptr, *MeshDir));
    		//
    		// if(LoadedMesh)
    		// {
    		// 	SpawnedABaseUnitActor->BaseMesh->SetStaticMesh(LoadedMesh);
    		// }
    		// ensure(SpawnedABaseUnitActor); // Ensure that the actor was successfully spawned

    		/* Spawn Manager에 생성 요청  */
    		USpawnManager* SpawnManager = GetGameInstance()->GetSubsystem<USpawnManager>();
    		if(CardName == TEXT("아리"))
    		{
    			SpawnManager->SpawnUnit(EUnitAttributeType::Ahri, 1, SpawnLocation, SpawnRotation);
    		}
    		else if (CardName == TEXT("미스포츈"))
    		{
    			SpawnManager->SpawnUnit(EUnitAttributeType::MissFortune, 1, SpawnLocation, SpawnRotation);
    		}
    		else if (CardName == TEXT("이렐리아"))
    		{
    			SpawnManager->SpawnUnit(EUnitAttributeType::Irelia, 1, SpawnLocation, SpawnRotation);
    		}
		    else
		    {
		    	SpawnManager->SpawnUnit(EUnitAttributeType::Ahri, 1, SpawnLocation, SpawnRotation);
		    }

    	}
    }
}


FReply UShopPanelSlot::NativeOnPreviewMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if(InMouseEvent.IsMouseButtonDown(EKeys::LeftMouseButton))
	{
		return UWidgetBlueprintLibrary::DetectDragIfPressed(InMouseEvent, this, EKeys::LeftMouseButton).NativeReply;
	}
	return Super::OnMouseButtonDown(InGeometry, InMouseEvent).NativeReply;
}

void UShopPanelSlot::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)
{
	UDragDropOperation* DragDropOperation = NewObject<UDragDropOperation>();
	DragDropOperation->Payload = this;
	DragDropOperation->DefaultDragVisual = this;
	OutOperation = DragDropOperation;
}


TSharedRef<SWidget> UShopPanelSlot::RebuildWidget()
{
	SetDesiredSizeInViewport(FVector2d(260,230));
	SetRenderTransformPivot(FVector2D(0,0));
	
	USizeBox* SizeBox = WidgetTree->ConstructWidget<USizeBox>(USizeBox::StaticClass(), TEXT("SizeBox"));
	SizeBox->SetWidthOverride(250);
	SizeBox->SetHeightOverride(220);
	WidgetTree->RootWidget = SizeBox;

	UBorder* Border = WidgetTree->ConstructWidget<UBorder>(UBorder::StaticClass(), TEXT("Border"));
	Border->SetPadding(FMargin(0,0,0,0));
	Border->SetHorizontalAlignment(HAlign_Fill);
	Border->SetVerticalAlignment(VAlign_Fill);
	// Border->SetBrushColor(FLinearColor(1, 0.184475, 0.291771, 0.564706));
	Border->SetBrushColor(FLinearColor::White);
	SizeBox->AddChild(Border);

	UHorizontalBox* BaseHorizontalBox = WidgetTree->ConstructWidget<UHorizontalBox>(UHorizontalBox::StaticClass(), TEXT("BaseHorizontalBox"));
	Border->AddChild(BaseHorizontalBox);
	
	UBorderSlot* BorderSlot = Cast<UBorderSlot>(Border->GetContentSlot());
	if(BorderSlot)
	{
		BorderSlot->SetHorizontalAlignment(HAlign_Fill);
		BorderSlot->SetVerticalAlignment(VAlign_Fill);
		BorderSlot->SetPadding(FMargin(0,0,0,0));
	}
	

	UVerticalBox* VerticalBox1 = AddVerticalBox(BaseHorizontalBox, FName("VerticalBox1"));
	AddBlockBox(BaseHorizontalBox, FName("Right_Block"),FLinearColor::Black, FMargin(3),EHorizontalAlignment::HAlign_Right, EVerticalAlignment::VAlign_Fill);
	
	UHorizontalBox* HorizontalBox1 = AddHorizontalBox(VerticalBox1, FName("HorizontalBox1"));
	AddBlockBox(VerticalBox1, FName("Bottom_Block"),FLinearColor::Black, FMargin(3), EHorizontalAlignment::HAlign_Fill, EVerticalAlignment::VAlign_Bottom);

	AddBlockBox(HorizontalBox1, FName("Left_Block"),FLinearColor::Black, FMargin(3),EHorizontalAlignment::HAlign_Left, EVerticalAlignment::VAlign_Fill);
	UVerticalBox* VerticalBox2 = AddVerticalBox(HorizontalBox1, FName("VerticalBox2"));

	AddBlockBox(VerticalBox2, FName("Top_Block"),FLinearColor::Black, FMargin(3),EHorizontalAlignment::HAlign_Fill, EVerticalAlignment::VAlign_Top);
	
	UBorder* Transparency = AddBlockBox(VerticalBox2, FName("Transparency"), FLinearColor(1, 0.184475, 0.291771, 0.564706),FMargin(0), EHorizontalAlignment::HAlign_Fill, EVerticalAlignment::VAlign_Fill);

	UCanvasPanel* CanvasPanel = WidgetTree->ConstructWidget<UCanvasPanel>(UCanvasPanel::StaticClass(), TEXT("CanvasPanel"));
	Transparency->AddChild(CanvasPanel);

	// 카드 중앙
	UBorderSlot* TransparencyBorderSlot = Cast<UBorderSlot>(Transparency->GetContentSlot());
	if(TransparencyBorderSlot)
	{
		TransparencyBorderSlot->SetHorizontalAlignment(HAlign_Fill);
		TransparencyBorderSlot->SetVerticalAlignment(VAlign_Fill);
		TransparencyBorderSlot->SetPadding(FMargin(0,0,0,0));
	}

	Image = WidgetTree->ConstructWidget<UImage>(UImage::StaticClass(), TEXT("Image"));
	UTexture2D* Texture = Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), nullptr, TEXT("/Script/Engine.Texture2D'/Game/UI/Image/TFT11_Irelia.TFT11_Irelia'")));
	if (Texture)
	{
		Image->SetBrushFromTexture(Texture);
	}
	UCanvasPanelSlot* ImageSlot = CanvasPanel->AddChildToCanvas(Image);
	ImageSlot->SetMinimum(FVector2D(0.5f,0.42f));
	ImageSlot->SetMaximum(FVector2D(0.5f,0.42f));
	ImageSlot->SetAlignment(FVector2d(0.5f,0.5f));
	ImageSlot->SetSize(FVector2D(250, 190.0f));
	ImageSlot->SetPosition(FVector2d(0.f,0.0f));

	UVerticalBox* VerticalboxInCanvas = AddVerticalBox(CanvasPanel, FName("VerticalboxInCanvas"));
	UCanvasPanelSlot* VerticalboxInCanvasSlot = Cast<UCanvasPanelSlot>(VerticalboxInCanvas->Slot);
	if(VerticalboxInCanvasSlot)
	{
		VerticalboxInCanvasSlot->SetMinimum(FVector2D(0.5f,0.73f));
		VerticalboxInCanvasSlot->SetMaximum(FVector2D(0.5f,0.73f));
		VerticalboxInCanvasSlot->SetAlignment(FVector2d(0.5f,0.5f));
		VerticalboxInCanvasSlot->SetSize(FVector2D(250, 122.0f));
		VerticalboxInCanvasSlot->SetPosition(FVector2d(0.0f,0.0f));
	}
	
	float SynergyFontSize = 16.0f;
	float NameAndPriceFontSize = 20.0f; 
	Synergy1 = AddTextBlock(VerticalboxInCanvas, FName("synergy1"), FText::FromString(TEXT("")), SynergyFontSize,1.0f, EHorizontalAlignment::HAlign_Left, VAlign_Center, FMargin(5,0,0,0));
	Synergy2 = AddTextBlock(VerticalboxInCanvas, FName("synergy2"), FText::FromString(TEXT("이야기꾼")), SynergyFontSize,1.0f, EHorizontalAlignment::HAlign_Left, VAlign_Center, FMargin(5,0,0,0));
	Synergy3 = AddTextBlock(VerticalboxInCanvas, FName("synergy3"), FText::FromString(TEXT("결투가")), SynergyFontSize,1.0f, EHorizontalAlignment::HAlign_Left, VAlign_Center, FMargin(5,0,0,0));

	BorderInVerticalBox = WidgetTree->ConstructWidget<UBorder>(UBorder::StaticClass(), TEXT("BorderInVerticalBox"));
	BorderInVerticalBox->SetBrushColor(FLinearColor(0.799479,0.239803,0.180057,1));
	BorderInVerticalBox->SetPadding(FMargin(2,0,2,0));
	UVerticalBoxSlot* BorderInVerticalBoxSlot = VerticalboxInCanvas->AddChildToVerticalBox(BorderInVerticalBox);
	FSlateChildSize BorderInVerticalBoxSlotChildSize;
	BorderInVerticalBoxSlotChildSize.Value = 1.2f;
	BorderInVerticalBoxSlot->SetSize(BorderInVerticalBoxSlotChildSize);
	
	UHorizontalBox* HorizontalboxInCanvas = AddHorizontalBox(BorderInVerticalBox, FName("HorizontalboxInCanvas"));
	UCanvasPanelSlot* HorizontalboxInCanvasSlot = Cast<UCanvasPanelSlot>(HorizontalboxInCanvas->Slot);
	if(HorizontalboxInCanvasSlot)
	{
		HorizontalboxInCanvasSlot->SetMinimum(FVector2D(0.5f,0.75f));
		HorizontalboxInCanvasSlot->SetMaximum(FVector2D(0.5f,0.75f));
		HorizontalboxInCanvasSlot->SetAlignment(FVector2d(0.5f,0.5f));
		HorizontalboxInCanvasSlot->SetSize(FVector2D(80, 30.0f));
		HorizontalboxInCanvasSlot->SetPosition(FVector2d(0.0f,0.0f));
	}
	
	Name = AddTextBlock(HorizontalboxInCanvas, FName("Name"), FText::FromString(TEXT("이렐리아")), NameAndPriceFontSize,1.0f, HAlign_Left, VAlign_Center, FMargin(5,0,0,0));
	Price = AddTextBlock(HorizontalboxInCanvas, FName("Price"), FText::FromString(TEXT("$22")), NameAndPriceFontSize,0.5f, HAlign_Right, VAlign_Center, FMargin(0,0,5,0));
	
	return Super::RebuildWidget();
}

template <typename T>
UHorizontalBox* UShopPanelSlot::AddHorizontalBox(T* ParentWidget, FName HorizontalBoxName)
{
	UHorizontalBox* HorizontalBox = WidgetTree->ConstructWidget<UHorizontalBox>(UHorizontalBox::StaticClass(), HorizontalBoxName);

	if(UBorder* Border = Cast<UBorder>(ParentWidget))
	{
		UBorderSlot* BorderSlot = Cast<UBorderSlot>(Border->AddChild(HorizontalBox));
		BorderSlot->SetHorizontalAlignment(HAlign_Fill);
		BorderSlot->SetVerticalAlignment(VAlign_Fill);
		BorderSlot->SetPadding(FMargin(0,0,0,0));
	}
	else if (UVerticalBox* VerticalBox = Cast<UVerticalBox>(ParentWidget))
	{
		UVerticalBoxSlot* BorderSlot = VerticalBox->AddChildToVerticalBox(HorizontalBox);
		FSlateChildSize ChildSize;
		ChildSize.Value = 1.0f;
		BorderSlot->SetSize(ChildSize);
		BorderSlot->SetHorizontalAlignment(HAlign_Fill);
		BorderSlot->SetVerticalAlignment(VAlign_Fill);
		BorderSlot->SetPadding(FMargin(0,0,0,0));
	}
	else if(UCanvasPanel* CanvasPanel = Cast<UCanvasPanel>(ParentWidget))
	{
		CanvasPanel->AddChild(HorizontalBox);
	}
	
	return HorizontalBox;
}

template <typename T>
UVerticalBox* UShopPanelSlot::AddVerticalBox(T* ParentWidget, FName VerticalBoxName)
{
	UVerticalBox* VerticalBox = WidgetTree->ConstructWidget<UVerticalBox>(UVerticalBox::StaticClass(), VerticalBoxName);

	if(UHorizontalBox* HorizontalBox = Cast<UHorizontalBox>(ParentWidget))
	{
		UHorizontalBoxSlot* VerticalBox1Slot = HorizontalBox->AddChildToHorizontalBox(VerticalBox);
		FSlateChildSize ChildSize;
		ChildSize.Value = 1.0f;
		VerticalBox1Slot->SetSize(ChildSize);
		VerticalBox1Slot->SetHorizontalAlignment(HAlign_Fill);
		VerticalBox1Slot->SetVerticalAlignment(VAlign_Fill);
		VerticalBox1Slot->SetPadding(FMargin(0,0,0,0));
	}
	else if(UCanvasPanel* CanvasPanel = Cast<UCanvasPanel>(ParentWidget))
	{
		CanvasPanel->AddChild(VerticalBox);
	}

	return VerticalBox;
}

template <typename T>
UBorder* UShopPanelSlot::AddBlockBox(T* ParentWidget, FName BorderName, FLinearColor Color, FMargin Padding, EHorizontalAlignment HorizontalAlignment, EVerticalAlignment VerticalAlignment)
{
	UBorder* Block = WidgetTree->ConstructWidget<UBorder>(UBorder::StaticClass(), BorderName);
	Block->SetPadding(Padding);
	Block->SetHorizontalAlignment(HAlign_Fill);
	Block->SetVerticalAlignment(VAlign_Fill);
	Block->SetBrushColor(Color);

	FSlateChildSize BlockSlotSize;
	if(BorderName == FName("Transparency"))
	{
		BlockSlotSize.SizeRule = ESlateSizeRule::Fill;
		BlockSlotSize.Value = 1.0f;
	}
	else
	{
		BlockSlotSize.SizeRule = ESlateSizeRule::Automatic;
	}

	if(UHorizontalBox* HorizontalBox = Cast<UHorizontalBox>(ParentWidget))
	{
		UHorizontalBoxSlot* BlockSlot = HorizontalBox->AddChildToHorizontalBox(Block);
		if(BlockSlot)
		{
			BlockSlot->SetSize(BlockSlotSize);
			BlockSlot->SetHorizontalAlignment(HorizontalAlignment);
			BlockSlot->SetVerticalAlignment(VerticalAlignment);
		}
	}
	else if(UVerticalBox* VerticalBox = Cast<UVerticalBox>(ParentWidget))
	{
		UVerticalBoxSlot* BlockSlot = VerticalBox->AddChildToVerticalBox(Block);
		if(BlockSlot)
		{
			BlockSlot->SetSize(BlockSlotSize);
			BlockSlot->SetHorizontalAlignment(HorizontalAlignment);
			BlockSlot->SetVerticalAlignment(VerticalAlignment);
		}
	}
	
	return Block;
}

template <typename T>
UTextBlock* UShopPanelSlot::AddTextBlock(T* ParentWidget, FName TextName, FText Text, float FontSize,  float BlockSize, EHorizontalAlignment HorizontalAlignment, EVerticalAlignment VerticalAlignment, FMargin Margin)
{
	UTextBlock* TextBlock = WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass(), TextName);
	TextBlock->SetText(Text);

	FSlateFontInfo FontInfo = FCoreStyle::GetDefaultFontStyle("Regular", FontSize);
	TextBlock->SetFont(FontInfo);
	
	FSlateChildSize BlockSlotSize;
	BlockSlotSize.SizeRule = ESlateSizeRule::Fill;
	BlockSlotSize.Value = BlockSize;

	if(UHorizontalBox* HorizontalBox = Cast<UHorizontalBox>(ParentWidget))
	{
		UHorizontalBoxSlot* BlockSlot = HorizontalBox->AddChildToHorizontalBox(TextBlock);
		if(BlockSlot)
		{
			BlockSlot->SetSize(BlockSlotSize);
			BlockSlot->SetHorizontalAlignment(HorizontalAlignment);
			BlockSlot->SetVerticalAlignment(VerticalAlignment);
			BlockSlot->SetPadding(Margin);
		}
	}
	else if(UVerticalBox* VerticalBox = Cast<UVerticalBox>(ParentWidget))
	{
		UVerticalBoxSlot* BlockSlot = VerticalBox->AddChildToVerticalBox(TextBlock);
		if(BlockSlot)
		{
			BlockSlot->SetSize(BlockSlotSize);
			BlockSlot->SetHorizontalAlignment(HorizontalAlignment);
			BlockSlot->SetVerticalAlignment(VerticalAlignment);
			BlockSlot->SetPadding(Margin);
		}
	}

	return TextBlock;
}

void UShopPanelSlot::NativeConstruct()
{
	Super::NativeConstruct();

	// 카드 정보를 UI에 반영
	UpdateCardInfo();
}

void UShopPanelSlot::UpdateCardInfo()
{
	if (Synergy1)
	{
		Synergy1->SetText(FText::FromString(Synergy1Text));
	}

	if (Synergy2)
	{
		Synergy2->SetText(FText::FromString(Synergy2Text));
	}

	if (Synergy3)
	{
		Synergy3->SetText(FText::FromString(Synergy3Text));
	}

	if (Name)
	{
		Name->SetText(FText::FromString(CardName));
	}

	if (Price)
	{
		Price->SetText(FText::FromString(PriceText));
	}


	if( Image && !ImageDir.IsEmpty())
	{
		UTexture2D* Texture = Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), nullptr, *ImageDir));
		if (Texture)
		{
			Image->SetBrushFromTexture(Texture);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Failed to load texture from path: %s"), *ImageDir);
		}
	}

	if(BorderInVerticalBox)
	{
		BorderInVerticalBox->SetBrushColor(BorderColor);
	}
}