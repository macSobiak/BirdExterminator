// Fill out your copyright notice in the Description page of Project Settings.


#include "WorldConfigFileParser.h"

WorldConfigFileParser::WorldConfigFileParser()
{
}

WorldConfigFileParser::~WorldConfigFileParser()
{
}

bool WorldConfigFileParser::TryGetWorldDataFromConfigFile(FWorldData* &WorldDataRef)
{
	FString FullFilePath = FPaths::ProjectConfigDir().Append(TEXT("WorldConfig.txt"));

	// We will use this FileManager to deal with the file.
	IPlatformFile& FileManager = FPlatformFileManager::Get().GetPlatformFile();

	FString FileContent;
	if (!FileManager.FileExists(*FullFilePath))
	{
		UE_LOG(LogTemp, Warning, TEXT("File not found, location: %s"),*FullFilePath);
		return false;
	}
	
	if(!FFileHelper::LoadFileToString(FileContent, *FullFilePath))
	{
		UE_LOG(LogTemp, Error, TEXT("Text load failed."));
		return false;
	}
	
	TArray<FString> FileContentSplit;

	if(FileContent.ParseIntoArrayLines(FileContentSplit) <= 1)
	{
		UE_LOG(LogTemp, Error, TEXT("Not enough data in config file!"));  
		return false;
	}
	
	TArray<FString> WorldSize;
	FileContentSplit[0].ParseIntoArray(WorldSize, TEXT(","));
			
	if(WorldSize.Num() != 2)
	{
		UE_LOG(LogTemp, Error, TEXT("Wrong world size!"));
		return false;
	}
	int ValueX = FCString::Atoi(*WorldSize[0]);
	int ValueY = FCString::Atoi(*WorldSize[1]);
	if(ValueX < 0 || ValueY < 0)
	{
		UE_LOG(LogTemp, Error, TEXT("World size cannot have a negative number!"));
		return false;
	}
	WorldDataRef = new FWorldData(ValueX, ValueY);
	if(FileContentSplit.Num() -1 != WorldDataRef->WorldSizeY)
	{
		UE_LOG(LogTemp, Error, TEXT("Not enough Building Heights in Y axis!"));
		return false;
	}
	
	for(int i = 1; i < FileContentSplit.Num(); ++i)
	{
		TArray<FString> BuildingHeights;
		FileContentSplit[i].ParseIntoArray(BuildingHeights, TEXT(","));
		if(BuildingHeights.Num() != WorldDataRef->WorldSizeX)
		{
			UE_LOG(LogTemp, Error, TEXT("Not enough Building Heights in X axis for row %d!"), i);
			return false;
		}
		for(int j = 0; j < BuildingHeights.Num(); ++j)
		{
			float Height = FCString::Atof(*BuildingHeights[j]);
			if(Height < 0)
			{
				UE_LOG(LogTemp, Error, TEXT("Height cannot be a negative number!"));
				return false;
			}
			WorldDataRef->BuildingHeightMatrix[i-1].push_back(Height);
		}
	}
	
	return true;
}
