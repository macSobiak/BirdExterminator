// Fill out your copyright notice in the Description page of Project Settings.


#include "BirdExterminatorGameMode.h"

#include "BirdExterminator/World/WorldBuilder.h"
#include "BirdExterminator/BirdsLogic/BirdsController.h"
#include "BirdExterminator/UI/UIController.h"


#include "Kismet/GameplayStatics.h"

void ABirdExterminatorGameMode::BeginPlay()
{
	Super::BeginPlay();

	FVector3f PlayableArea;
	FString ErrorMessage;
	WorldBuilder = Cast<AWorldBuilder>(UGameplayStatics::GetActorOfClass(this, AWorldBuilder::StaticClass()));
	BirdsController = Cast<ABirdsController>(UGameplayStatics::GetActorOfClass(this, ABirdsController::StaticClass()));
	UIController = Cast<AUIController>(UGameplayStatics::GetActorOfClass(this, AUIController::StaticClass()));
	
	if(!WorldBuilder->GenerateWorld(PlayableArea, ErrorMessage))
	{
		UIController->ShowPopup(ErrorMessage);
		return;
	}
	
	BirdsController->Initialize(PlayableArea, 10);

	BirdsController->OnBirdCountChangedEvent.AddUObject(this, &ABirdExterminatorGameMode::CheckWinLooseConditions);
	BirdsController->OnPredatorCountChangedEvent.AddUObject(this, &ABirdExterminatorGameMode::CheckWinLooseConditions);
	BirdsController->OnPredatorAliveCountChangedEvent.AddUObject(this, &ABirdExterminatorGameMode::CheckWinLooseConditions);

	UIController->InitializeInterfaceElements(BirdsController);
}

void ABirdExterminatorGameMode::CheckWinLooseConditions(const int& _)
{
	if(BirdsController->PreyBirdsAlive == 0)
	{
		UIController->ShowPopup(TEXT("ALL BIRDS EXTERMINATED!\nCONGRATULATIONS!"));
	}
	else if (BirdsController->PredatorBirdsAlive == 0 && BirdsController->PredatorBirdsAvailable == 0)
	{
		UIController->ShowPopup(FString::Printf(TEXT("THERE ARE %d BIRDS LEFT TO EXTERMINATE!\nYOU LOST!"), BirdsController->PreyBirdsAlive));
	}
}
