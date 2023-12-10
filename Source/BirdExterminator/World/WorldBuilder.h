// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WorldBuilder.generated.h"

UCLASS()
class BIRDEXTERMINATOR_API AWorldBuilder : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWorldBuilder();
	bool GenerateWorld(FVector3f &PlayableArea, FString &ErrorMessage);


private:

	void SpawnInvisibleWall(const FVector &SpawnLocation, const FVector &ScaleVector);
	void SpawnInvisibleWalls(const FVector &WorldSize, const float& MaxBuildingHeight);
	void ResetPlayerCharacter(const uint16 &WorldSizeX, const uint16 &WorldSizeY);
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<AActor> BuildingActor;
	UPROPERTY(EditAnywhere)
	TSubclassOf<AActor> InvisibleWallActor;
	
	UPROPERTY(EditAnywhere)
	float DistanceBetweenBuildings = 500;

	UPROPERTY(EditAnywhere)
	float BuildingScaleX = 3;
	UPROPERTY(EditAnywhere)
	float BuildingScaleY = 2;
	float BuildingSize = 100;

};