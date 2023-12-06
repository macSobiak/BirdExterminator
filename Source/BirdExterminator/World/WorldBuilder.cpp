// Fill out your copyright notice in the Description page of Project Settings.


#include "WorldBuilder.h"

#include "WorldConfigFileParser.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AWorldBuilder::AWorldBuilder()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void AWorldBuilder::BeginPlay()
{
	Super::BeginPlay();

	WorldConfigFileParser* FileParser = new WorldConfigFileParser;
	FWorldData WorldData;
	if(!FileParser->TryGetWorldDataFromConfigFile(WorldData))
		return;
	
	float BuildingSizeX = 100 * BuildingScaleX;
	float BuildingSizeY = 100 * BuildingScaleY;
	
	FVector WorldSize = FVector( WorldData.WorldSizeX * (BuildingSizeX + DistanceBetweenBuildings) + DistanceBetweenBuildings,
								WorldData.WorldSizeY * (BuildingSizeY + DistanceBetweenBuildings) + DistanceBetweenBuildings,
								1);
	
	SetActorScale3D(WorldSize / 100);
	const float InitialPosX = (WorldSize.X / 2.0f) - BuildingSizeX / 2 - DistanceBetweenBuildings;
	const float InitialPosY = (WorldSize.Y / 2.0f) - BuildingSizeY / 2 - DistanceBetweenBuildings;
	
	FVector SpawnPosition = FVector(InitialPosX, InitialPosY, 0);

	for (int i = 0; i < WorldData.BuildingHeightMatrix.size(); ++i)
	{
		SpawnPosition.X = InitialPosX;

		for (int j = 0; j <  WorldData.BuildingHeightMatrix[i].size(); ++j)
		{
			auto BuildingSpawned = GetWorld()->SpawnActor(BuildingActor, &SpawnPosition);
			BuildingSpawned->SetActorScale3D(FVector(BuildingScaleX, BuildingScaleY, WorldData.BuildingHeightMatrix[i][j]));
			SpawnPosition.X -= (BuildingSizeX + DistanceBetweenBuildings);

		}
		SpawnPosition.Y -= (BuildingSizeY + DistanceBetweenBuildings);
	}

	auto Character = UGameplayStatics::GetPlayerCharacter(this, 0);

	bool IsMiddlePointOccupied = (WorldData.WorldSizeX % 2) > 0 && (WorldData.WorldSizeY % 2) > 0;
	Character->SetActorLocation(FVector(IsMiddlePointOccupied ? (BuildingSizeX + DistanceBetweenBuildings)/2 : 0,
		IsMiddlePointOccupied ? (BuildingSizeY + DistanceBetweenBuildings)/2 : 0,
		100));
}