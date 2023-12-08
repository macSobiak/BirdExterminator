// Fill out your copyright notice in the Description page of Project Settings.


#include "BirdExterminatorGameMode.h"

#include "BirdExterminator/World/WorldBuilder.h"
#include "BirdExterminator/World/BirdFlock.h"


#include "Kismet/GameplayStatics.h"

void ABirdExterminatorGameMode::BeginPlay()
{
	Super::BeginPlay();

	WorldBuilder = Cast<AWorldBuilder>(UGameplayStatics::GetActorOfClass(this, AWorldBuilder::StaticClass()));

	WorldBuilder->GenerateWorld(PlayableArea);
	FVector RandomSpawnPoint;
	for(int i =0; i < 10; ++i)
	{
		RandomSpawnPoint.X = FMath::RandRange(-PlayableArea.X / 2, PlayableArea.X / 2);
		RandomSpawnPoint.Y = FMath::RandRange(-PlayableArea.Y / 2, PlayableArea.Y / 2);
		RandomSpawnPoint.Z = FMath::RandRange(PlayableArea.Z/2, PlayableArea.Z);
		
		Cast<ABirdFlock>(GetWorld()->SpawnActor(BirdFlockBlueprint, &RandomSpawnPoint))->Initialize(PlayableArea);
	}


	
}
