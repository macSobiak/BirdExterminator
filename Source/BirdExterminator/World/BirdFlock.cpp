// Fill out your copyright notice in the Description page of Project Settings.


#include "BirdFlock.h"
#include "Kismet/KismetMathLibrary.h"
#include "Bird.h"

// Sets default values
ABirdFlock::ABirdFlock()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

void ABirdFlock::Initialize(const FVector3f &PlayableAreaRef, const int &BirdCount, ABirdsController *BirdsControllerInstance)
{
	BirdVector.reserve(BirdCount);
	PlaceInFlockPosition.reserve(BirdCount);

	PlayableArea = PlayableAreaRef;
	BirdsController = BirdsControllerInstance;
	
	SpawnBirds(BirdCount);
	GenerateNewDestinationPoint();

	IsInitialized = true;
}

inline void ABirdFlock::SpawnBirds(const int &BirdCount)
{
	FVector SpawnPositionOffset = FVector::Zero();

	const float Angle = 360 / (BirdCount - 1);
	for (int i = 0; i < BirdCount; ++i)
	{
		if(i != 0)
		{
			SpawnPositionOffset.Y = 200 * FMath::Cos(FMath::DegreesToRadians(Angle*(i-1))) +  FMath::RandRange(-10, 10);
			SpawnPositionOffset.X = 200 * FMath::Sin(FMath::DegreesToRadians(Angle*(i-1))) +  FMath::RandRange(-10, 10);
		}

		SpawnPositionOffset.Z = FMath::RandRange(-200, 200);

		auto SpawnPosition = GetActorLocation() + SpawnPositionOffset;
		const auto BirdSpawned = Cast<ABird>(GetWorld()->SpawnActor(BirdBlueprint, &SpawnPosition));
		PlaceInFlockPosition.push_back(SpawnPositionOffset + FVector(0,10,0));

		BirdSpawned->InitializeCommonObjects(BirdsController);
		BirdSpawned->InitializeAsPrey(this, i, PlayableArea);
		BirdSpawned->OnBirdDestroyedEvent.AddUObject(this, &ABirdFlock::HandleBirdDestroyed);
		BirdVector.push_back(BirdSpawned);
	}
}

void ABirdFlock::HandleBirdDestroyed(ABird* Bird)
{
	Bird->OnBirdDestroyedEvent.RemoveAll(this);
	std::erase(BirdVector, Bird);
	OnBirdInFlockDestroyedChanged.Broadcast();

	if(BirdVector.size() == 0)
	{
		Destroy();
	}
}

inline void ABirdFlock::GenerateNewDestinationPoint()
{
	FlockFlightDestination = FVector(FMath::RandRange(-PlayableArea.X / 2, PlayableArea.X / 2),
	FMath::RandRange(-PlayableArea.Y / 2, PlayableArea.Y / 2),
	FMath::RandRange(10.0f, PlayableArea.Z));
}

FVector ABirdFlock::GetPlaceInFlock(const int& PlaceInFlock)
{
	return GetActorLocation() + PlaceInFlockPosition[PlaceInFlock];
}

void ABirdFlock::Destroyed()
{
	Super::Destroyed();
	OnBirdInFlockDestroyedChanged.Clear();
}

// Called every frame
void ABirdFlock::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if(!IsInitialized)
		return;
	
	SetActorLocation(FMath::VInterpConstantTo(GetActorLocation(), FlockFlightDestination, DeltaTime, 500));

	if(GetActorLocation() == FlockFlightDestination)
	{
		GenerateNewDestinationPoint();
	}
}

