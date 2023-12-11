// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * struct containing all data required to build a world with flat plane as ground and scale the cubes to the size of buildings
 */
struct FWorldData
{
	FWorldData(){};
	FWorldData(const uint8& WorldSizeXRef, const uint8& WorldSizeYRef) : WorldSizeX(WorldSizeXRef), WorldSizeY(WorldSizeYRef)
	{
		BuildingHeightMatrix.reserve(WorldSizeY);
		for (int i = 0; i < WorldSizeY; i++)
		{
			BuildingHeightMatrix.push_back(std::vector<float>());
			BuildingHeightMatrix[i].reserve(WorldSizeX);
		}
	}
	std::vector<std::vector<float>> BuildingHeightMatrix;

	uint16 WorldSizeX = 5; 
	uint16 WorldSizeY = 4;

};

/**
 * class responsible for parsing config file located in project config directory, named WorldConfig.txt and returning the data for world building
 */
class BIRDEXTERMINATOR_API WorldConfigFileParser
{
	bool ReturnErrorAndSetMessage(const FString &ErrorMessageLog, FString &OutErrorMessage);
public:
	WorldConfigFileParser();
	~WorldConfigFileParser();

	bool TryGetWorldDataFromConfigFile(FWorldData &WorldDataRef, FString &ErrorMessage);
};
