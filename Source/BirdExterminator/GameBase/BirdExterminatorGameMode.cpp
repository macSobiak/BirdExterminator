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
	BirdsController->Initialize(PlayableArea, 10);;

	UIController = Cast<AUIController>(UGameplayStatics::GetActorOfClass(this, AUIController::StaticClass()));
	UIController->InitializeInterfaceElements(BirdsController);
}
