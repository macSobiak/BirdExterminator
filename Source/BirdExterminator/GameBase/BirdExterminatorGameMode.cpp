// Fill out your copyright notice in the Description page of Project Settings.


#include "BirdExterminatorGameMode.h"

#include "BirdExterminator/World/WorldBuilder.h"
#include "BirdExterminator/BirdsLogic/BirdsController.h"


#include "Kismet/GameplayStatics.h"

void ABirdExterminatorGameMode::BeginPlay()
{
	Super::BeginPlay();

	WorldBuilder = Cast<AWorldBuilder>(UGameplayStatics::GetActorOfClass(this, AWorldBuilder::StaticClass()));

	WorldBuilder->GenerateWorld(PlayableArea);
	
	BirdsController = Cast<ABirdsController>(GetWorld()->SpawnActor(BirdsControllerBlueprint));
	BirdsController->Initialize(PlayableArea, 10);;


	
}
