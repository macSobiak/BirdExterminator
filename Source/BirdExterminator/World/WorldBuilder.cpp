// Fill out your copyright notice in the Description page of Project Settings.


#include "WorldBuilder.h"

#include "WorldConfigFileParser.h"
#include "Engine/BlockingVolume.h"
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

	float MaxBuildingHeight = 0;
	FVector SpawnPosition = FVector(InitialPosX, InitialPosY, 0);

	for (int i = 0; i < WorldData.BuildingHeightMatrix.size(); ++i)
	{
		SpawnPosition.X = InitialPosX;

		for (int j = 0; j <  WorldData.BuildingHeightMatrix[i].size(); ++j)
		{
			auto BuildingSpawned = GetWorld()->SpawnActor(BuildingActor, &SpawnPosition);
			auto BuildingHeight = WorldData.BuildingHeightMatrix[i][j];
			BuildingSpawned->SetActorScale3D(FVector(BuildingScaleX, BuildingScaleY, BuildingHeight));
			SpawnPosition.X -= (BuildingSizeX + DistanceBetweenBuildings);

			if(MaxBuildingHeight < BuildingHeight)
			{
				MaxBuildingHeight = BuildingHeight;
			}

		}
		SpawnPosition.Y -= (BuildingSizeY + DistanceBetweenBuildings);
	}

	auto Character = UGameplayStatics::GetPlayerCharacter(this, 0);

	bool IsMiddlePointOccupied = (WorldData.WorldSizeX % 2) > 0 && (WorldData.WorldSizeY % 2) > 0;
	Character->SetActorLocation(FVector(IsMiddlePointOccupied ? (BuildingSizeX + DistanceBetweenBuildings)/2 : 0,
		IsMiddlePointOccupied ? (BuildingSizeY + DistanceBetweenBuildings)/2 : 0,
		100));

	SpawnInvisibleWalls(WorldSize, MaxBuildingHeight);
}

void AWorldBuilder::SpawnInvisibleWalls(const FVector &WorldSize, const float& MaxBuildingHeight)
{
	FVector Bounds = FVector::Zero();
	FVector Scale = FVector::One();

	Bounds.X = -WorldSize.X / 2;
	Scale.Y = WorldSize.Y / 100;
	Scale.Z = DistanceBetweenBuildings/100 + MaxBuildingHeight;
	SpawnInvisibleWall(Bounds, Scale);

	Bounds.X = WorldSize.X / 2;
	SpawnInvisibleWall(Bounds, Scale);

	Bounds.X = 0;
	Bounds.Y = -WorldSize.Y / 2;
	Scale.X = WorldSize.X / 100;
	Scale.Y = 1;
	SpawnInvisibleWall(Bounds, Scale);

	Bounds.Y = WorldSize.Y / 2;
	SpawnInvisibleWall(Bounds, Scale);

	Bounds.X = 0;
	Bounds.Y = 0;
	Bounds.Z = MaxBuildingHeight * 100 + DistanceBetweenBuildings;
	Scale.Y = WorldSize.Y / 100;
	Scale.Z = 1;
	SpawnInvisibleWall(Bounds, Scale);
}

void AWorldBuilder::SpawnInvisibleWall(const FVector& SpawnLocation, const FVector &ScaleVector)
{
	GetWorld()->SpawnActor(InvisibleWallActor, &SpawnLocation)->SetActorScale3D(ScaleVector);
}


