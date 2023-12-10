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

	void GenerateNewDestinationPoint();
	void HandleBirdDestroyed(ABird* Bird);
public:	
	// Sets default values for this actor's properties
	ABirdFlock();
	void SpawnBirds(const int &BirdCount);
	void Initialize(const FVector3f &PlayableAreaRef, const int &BirdCount, ABirdsController *BirdsControllerInstance);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void Destroyed() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	FOnBirdInFlockDestroyed OnBirdInFlockDestroyedChanged;
	FVector GetPlaceInFlock(const int &PlaceInFlock);

	UPROPERTY(EditAnywhere)
	TSubclassOf<ABird> BirdBlueprint;
	TArray<ABird*> BirdArray;
	TArray<FVector> PlaceInFlockMap;

	FVector FlockFlightDestination;
	UPROPERTY(EditAnywhere)
	FVector3f PlayableArea;
	
	UPROPERTY(EditAnywhere)
	ABirdsController *BirdsController;

	bool IsInitialized = false;
	int TrackedBirdCount = 0;

};
