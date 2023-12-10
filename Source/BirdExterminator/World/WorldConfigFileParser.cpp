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
		return ReturnErrorAndSetMessage(FString::Printf(TEXT("File not found, location: %s"),*FullFilePath), ErrorMessage);
	}
	
	if(!FFileHelper::LoadFileToString(FileContent, *FullFilePath))
	{
		return ReturnErrorAndSetMessage(FString::Printf(TEXT("Text load failed.")), ErrorMessage);
	}
	
	TArray<FString> FileContentSplit;
	if(FileContent.ParseIntoArrayLines(FileContentSplit) <= 1)
	{
		return ReturnErrorAndSetMessage(FString::Printf(TEXT("Not enough data in config file!")), ErrorMessage);
	}
	
	TArray<FString> WorldSize;
	FileContentSplit[0].ParseIntoArray(WorldSize, TEXT(","));
			
	if(WorldSize.Num() != 2)
	{
		return ReturnErrorAndSetMessage(FString::Printf(TEXT("Wrong world size!")), ErrorMessage);
	}
	int ValueX = FCString::Atoi(*WorldSize[0]);
	int ValueY = FCString::Atoi(*WorldSize[1]);
	if(ValueX < 0 || ValueY < 0)
	{
		return ReturnErrorAndSetMessage(FString::Printf(TEXT("World size cannot have a negative number!")), ErrorMessage);
	}
	WorldDataRef = FWorldData(ValueX, ValueY);
	if(FileContentSplit.Num() -1 != WorldDataRef.WorldSizeY)
	{
		return ReturnErrorAndSetMessage(FString::Printf(TEXT("Not enough Building Heights in Y axis!")), ErrorMessage);
	}
	
	for(int i = 1; i < FileContentSplit.Num(); ++i)
	{
		TArray<FString> BuildingHeights;
		FileContentSplit[i].ParseIntoArray(BuildingHeights, TEXT(","));
		if(BuildingHeights.Num() != WorldDataRef.WorldSizeX)
		{
			return ReturnErrorAndSetMessage(FString::Printf(TEXT("Not enough Building Heights in X axis for row %d!"), i), ErrorMessage);
		}
		for(int j = 0; j < BuildingHeights.Num(); ++j)
		{
			float Height = FCString::Atof(*BuildingHeights[j]);
			if(Height < 0)
			{
				return ReturnErrorAndSetMessage(FString::Printf(TEXT("Height cannot be a negative number!")), ErrorMessage);
			}
			
			WorldDataRef.BuildingHeightMatrix[i-1].push_back(Height);
		}
	}
	
	return true;
}

inline bool WorldConfigFileParser::ReturnErrorAndSetMessage(const FString &ErrorMessageLog, FString &OutErrorMessage)
{
	OutErrorMessage = ErrorMessageLog;
	UE_LOG(LogTemp, Error, TEXT("%s"), *ErrorMessageLog);
	
	return false;
}