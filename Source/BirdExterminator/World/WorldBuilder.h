// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WorldConfigFileParser.h"
#include "GameFramework/Actor.h"
#include "WorldBuilder.generated.h"

UCLASS()
class BIRDEXTERMINATOR_API AWorldBuilder : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWorldBuilder();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere)
	TSubclassOf<AActor> BuildingActor;
	
	UPROPERTY(EditAnywhere)
	float DistanceBetweenBuildings = 500;

	UPROPERTY(EditAnywhere)
	float BuildingScaleX = 3;
	UPROPERTY(EditAnywhere)
	float BuildingScaleY = 2;

};
