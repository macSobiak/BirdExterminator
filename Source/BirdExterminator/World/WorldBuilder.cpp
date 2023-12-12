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

bool AWorldBuilder::GenerateWorld(FVector3f &PlayableArea, FString &ErrorMessage)
{
	WorldConfigFileParser* FileParser = new WorldConfigFileParser;
	FWorldData WorldData;
	if(!FileParser->TryGetWorldDataFromConfigFile(WorldData, ErrorMessage))
	{
		delete FileParser;
		FileParser = nullptr;
		return false;
	}
	
	float BuildingSizeX = BuildingSize * BuildingScaleX;
	float BuildingSizeY = BuildingSize * BuildingScaleY;
	
	FVector WorldSize = FVector( WorldData.WorldSizeX * (BuildingSizeX + DistanceBetweenBuildings) + DistanceBetweenBuildings,
								WorldData.WorldSizeY * (BuildingSizeY + DistanceBetweenBuildings) + DistanceBetweenBuildings,
								1);
	
	SetActorScale3D(WorldSize / BuildingSize);

	const float InitialPosX = (WorldSize.X / 2.0f) - BuildingSizeX / 2 - DistanceBetweenBuildings;
	const float InitialPosY = (WorldSize.Y / 2.0f) - BuildingSizeY / 2 - DistanceBetweenBuildings;

	float MaxBuildingHeight = 500;//this should be the lowest possible playable area
	FVector SpawnPosition = FVector(InitialPosX, InitialPosY, 0);

	for (int i = 0; i < WorldData.BuildingHeightMatrix.size(); ++i)
	{
		SpawnPosition.X = InitialPosX;

		for (int j = 0; j <  WorldData.BuildingHeightMatrix[i].size(); ++j)
		{
			auto BuildingSpawned = GetWorld()->SpawnActor(BuildingActor, &SpawnPosition);
			float &BuildingHeight = WorldData.BuildingHeightMatrix[i][j];
			BuildingSpawned->SetActorScale3D(FVector(BuildingScaleX, BuildingScaleY, BuildingHeight/100));
			
			SpawnPosition.X -= (BuildingSizeX + DistanceBetweenBuildings);

			if(MaxBuildingHeight < BuildingHeight)
			{
				MaxBuildingHeight = BuildingHeight;
			}

		}
		SpawnPosition.Y -= (BuildingSizeY + DistanceBetweenBuildings);
	}

	PlayableArea = FVector3f(WorldSize.X, WorldSize.Y, MaxBuildingHeight);
	SpawnInvisibleWalls(WorldSize, MaxBuildingHeight);
	ResetPlayerCharacter(WorldData.WorldSizeX, WorldData.WorldSizeY);

	delete FileParser;
	FileParser = nullptr;
	return true;
}


inline void AWorldBuilder::ResetPlayerCharacter(const uint16 &WorldSizeX, const uint16 &WorldSizeY)
{
	auto Character = UGameplayStatics::GetPlayerCharacter(this, 0);

	bool IsMiddlePointOccupied = (WorldSizeX % 2) > 0 && (WorldSizeY % 2) > 0;
	Character->SetActorLocation(FVector(IsMiddlePointOccupied ? (BuildingSize * BuildingScaleX + DistanceBetweenBuildings)/2 : 0,
		IsMiddlePointOccupied ? (BuildingSize * BuildingScaleY + DistanceBetweenBuildings)/2 : 0,
		100));
}


inline void AWorldBuilder::SpawnInvisibleWalls(const FVector &WorldSize, const float& MaxBuildingHeight)
{
	FVector Bounds = FVector::Zero();
	FVector Scale = FVector::One();

	Bounds.X = -WorldSize.X / 2;
	Scale.Y = WorldSize.Y / 100;
	Scale.Z = DistanceBetweenBuildings/100 + MaxBuildingHeight/100;
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
}

inline void AWorldBuilder::SpawnInvisibleWall(const FVector& SpawnLocation, const FVector &ScaleVector)
{
	GetWorld()->SpawnActor(InvisibleWallActor, &SpawnLocation)->SetActorScale3D(ScaleVector);
}


