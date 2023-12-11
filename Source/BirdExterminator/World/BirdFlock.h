// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BirdFlock.generated.h"

class ABird;
class ABirdsController;

DECLARE_EVENT(ABirdFlock, FOnBirdInFlockDestroyed);

UCLASS()
class BIRDEXTERMINATOR_API ABirdFlock : public AActor
{
	GENERATED_BODY()

public:	
	// Sets default values for this actor's properties
	ABirdFlock();
	
	void Initialize(const FVector3f &PlayableAreaRef, const int &BirdCount, ABirdsController *BirdsControllerInstance);
	virtual void Tick(float DeltaTime) override;
	FVector GetPlaceInFlock(const int &PlaceInFlock);
	
private:
	void GenerateNewDestinationPoint();
	void HandleBirdDestroyed(ABird* Bird);
	void SpawnBirds(const int &BirdCount);
		
protected:
	virtual void Destroyed() override;

public:
	FOnBirdInFlockDestroyed OnBirdInFlockDestroyedChanged;
	UPROPERTY(EditAnywhere)
	TSubclassOf<ABird> BirdBlueprint;
	std::vector<ABird*> BirdVector;

private:
	std::vector<FVector> PlaceInFlockPosition;
	FVector FlockFlightDestination;
	FVector3f PlayableArea;
	
	UPROPERTY()
	ABirdsController *BirdsController;
	bool IsInitialized = false;

};
