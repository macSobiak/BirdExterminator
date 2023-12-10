// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
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

	size_t WorldSizeX = 5; 
	size_t WorldSizeY = 4;

};

class BIRDEXTERMINATOR_API WorldConfigFileParser
{
	void LogErrorMessage(const FString &ErrorMessage);
public:
	WorldConfigFileParser();
	~WorldConfigFileParser();

	bool TryGetWorldDataFromConfigFile(FWorldData &WorldDataRef, FString &ErrorMessage);
};
