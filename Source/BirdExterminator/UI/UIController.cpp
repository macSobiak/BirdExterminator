// Fill out your copyright notice in the Description page of Project Settings.


#include "UIController.h"

#include "BirdsAliveCounter.h"
#include "ShotPredatorsCounter.h"
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
	
	PreyBirdCounter = CreateWidget<UBirdsAliveCounter>(PlayerController, PreyBirdCounterWidget);
	PredatorBirdsCounter = CreateWidget<UShotPredatorsCounter>(PlayerController, PredatorBirdsCounterWidget);
	
	Crosshair = CreateWidget<UUserWidget>(PlayerController, CrosshairWidget);
	
	PreyBirdCounter->InitializeElement(BirdsController->PreyBirdsAlive, BirdsController->PreyBirdsAlive);
	PreyBirdCounter->RegisterToControllerEvents(BirdsController);
	
	PredatorBirdsCounter->InitializeElement(BirdsController->PredatorBirdsAvailable, BirdsController->PredatorBirdsAvailable);
	PredatorBirdsCounter->RegisterToControllerEvents(BirdsController);
	
	PreyBirdCounter->AddToViewport();
	PredatorBirdsCounter->AddToViewport();
	
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

