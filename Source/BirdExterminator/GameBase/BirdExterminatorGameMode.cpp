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
	if(!WorldBuilder->GenerateWorld(PlayableArea, ErrorMessage))
	{
		//show UI prompt
		return;
	}
	
	BirdsController = Cast<ABirdsController>(UGameplayStatics::GetActorOfClass(this, ABirdsController::StaticClass()));
	BirdsController->Initialize(PlayableArea, 1);

	BirdsController->OnBirdCountChangedEvent.AddUObject(this, &ABirdExterminatorGameMode::CheckWinLooseConditions);
	BirdsController->OnPredatorCountChangedEvent.AddUObject(this, &ABirdExterminatorGameMode::CheckWinLooseConditions);
	BirdsController->OnPredatorAliveCountChangedEvent.AddUObject(this, &ABirdExterminatorGameMode::CheckWinLooseConditions);

	UIController = Cast<AUIController>(UGameplayStatics::GetActorOfClass(this, AUIController::StaticClass()));
	UIController->InitializeInterfaceElements(BirdsController);
}

void ABirdExterminatorGameMode::CheckWinLooseConditions(const int& _)
{
	if(BirdsController->PreyBirdsAlive == 0)
	{
		UE_LOG(LogTemp, Display, TEXT("-------------WON-------------"))
		//WIN
	}
	else if (BirdsController->PredatorBirdsAlive == 0 && BirdsController->PredatorBirdsAvailable == 0)
	{
		UE_LOG(LogTemp, Display, TEXT("-------------LOST-------------"))
		//LOSE
	}
}
