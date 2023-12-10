// Fill out your copyright notice in the Description page of Project Settings.


#include "UIController.h"

#include "GameDataTrackingPanel.h"
#include "PromptWithButtons.h"
#include "BirdExterminator/BirdsLogic/BirdsController.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AUIController::AUIController()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

void AUIController::InitializeInterfaceElements(ABirdsController* BirdsControllerInstance)
{
	BirdsController = BirdsControllerInstance;
	PlayerController =  UGameplayStatics::GetPlayerController(GetWorld(), 0);
	
	GameDataTrackingPanel = CreateWidget<UGameDataTrackingPanel>(PlayerController, GameDataTrackingPanelWidget);
	Crosshair = CreateWidget<UUserWidget>(PlayerController, CrosshairWidget);

	GameDataTrackingPanel->InitializePanel(BirdsController);
	GameDataTrackingPanel->AddToViewport();
	
	Crosshair->AddToViewport();
}

// Called when the game starts or when spawned
void AUIController::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AUIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AUIController::ShowPopup(const FString& PopupMessage)
{
	if(!PlayerController)
		PlayerController =  UGameplayStatics::GetPlayerController(GetWorld(), 0);

	PromptWithButtons = CreateWidget<UPromptWithButtons>(PlayerController, PopupWidget);
	
	PromptWithButtons->InitializePrompt(PopupMessage);
	PromptWithButtons->AddToViewport();

	PlayerController->SetInputMode(FInputModeUIOnly());
	PlayerController->SetShowMouseCursor(true);
}

