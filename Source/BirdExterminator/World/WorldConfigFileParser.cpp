// Fill out your copyright notice in the Description page of Project Settings.


#include "WorldConfigFileParser.h"

WorldConfigFileParser::WorldConfigFileParser()
{
}

WorldConfigFileParser::~WorldConfigFileParser()
{
}

bool WorldConfigFileParser::TryGetWorldDataFromConfigFile(FWorldData &WorldDataRef, FString &ErrorMessage)
{
	const FString FullFilePath = FPaths::ProjectConfigDir().Append(TEXT("WorldConfig.txt"));
	// We will use this FileManager to deal with the file.
	IPlatformFile& FileManager = FPlatformFileManager::Get().GetPlatformFile();

	FString FileContent;
	if (!FileManager.FileExists(*FullFilePath))
	{
		ErrorMessage = FString::Printf(TEXT("File not found, location: %s"),*FullFilePath);
		LogErrorMessage(ErrorMessage);
		return false;
	}
	
	if(!FFileHelper::LoadFileToString(FileContent, *FullFilePath))
	{
		ErrorMessage = FString::Printf(TEXT("Text load failed."));
		LogErrorMessage(ErrorMessage);
		return false;
	}
	
	TArray<FString> FileContentSplit;

	if(FileContent.ParseIntoArrayLines(FileContentSplit) <= 1)
	{
		ErrorMessage = FString::Printf(TEXT("Not enough data in config file!"));
		LogErrorMessage(ErrorMessage);
		return false;
	}
	
	TArray<FString> WorldSize;
	FileContentSplit[0].ParseIntoArray(WorldSize, TEXT(","));
			
	if(WorldSize.Num() != 2)
	{
		ErrorMessage = FString::Printf(TEXT("Wrong world size!"));
		LogErrorMessage(ErrorMessage);
		return false;
	}
	int ValueX = FCString::Atoi(*WorldSize[0]);
	int ValueY = FCString::Atoi(*WorldSize[1]);
	if(ValueX < 0 || ValueY < 0)
	{
		ErrorMessage = FString::Printf(TEXT("World size cannot have a negative number!"));
		LogErrorMessage(ErrorMessage);
		return false;
	}
	WorldDataRef = FWorldData(ValueX, ValueY);
	if(FileContentSplit.Num() -1 != WorldDataRef.WorldSizeY)
	{
		ErrorMessage = FString::Printf(TEXT("Not enough Building Heights in Y axis!"));
		LogErrorMessage(ErrorMessage);
		return false;
	}
	
	for(int i = 1; i < FileContentSplit.Num(); ++i)
	{
		TArray<FString> BuildingHeights;
		FileContentSplit[i].ParseIntoArray(BuildingHeights, TEXT(","));
		if(BuildingHeights.Num() != WorldDataRef.WorldSizeX)
		{
			ErrorMessage = FString::Printf(TEXT("Not enough Building Heights in X axis for row %d!"), i);
			LogErrorMessage(ErrorMessage);
			return false;
		}
		for(int j = 0; j < BuildingHeights.Num(); ++j)
		{
			float Height = FCString::Atof(*BuildingHeights[j]);
			if(Height < 0)
			{
				ErrorMessage = FString::Printf(TEXT("Height cannot be a negative number!"));
				LogErrorMessage(ErrorMessage);
				return false;
			}
			WorldDataRef.BuildingHeightMatrix[i-1].push_back(Height);
		}
	}
	
	return true;
}

void WorldConfigFileParser::LogErrorMessage(const FString &ErrorMessage)
{
	UE_LOG(LogTemp, Error, TEXT("%s"), *ErrorMessage);

}